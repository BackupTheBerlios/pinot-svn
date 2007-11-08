/*
 *  Copyright 2005,2006 Fabrice Colin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "config.h"
#include <stdlib.h>
#ifdef HAVE_STATFS
#include <sys/vfs.h>
#define CHECK_DISK_SPACE 1
#else
#ifdef HAVE_STATVFS
#include <sys/statvfs.h>
#define CHECK_DISK_SPACE 1
#endif
#endif
#include <iostream>
#include <glibmm/ustring.h>
#include <glibmm/stringutils.h>
#include <glibmm/convert.h>
#include <glibmm/thread.h>
#include <glibmm/random.h>

#include "Url.h"
#include "MonitorFactory.h"
#include "CrawlHistory.h"
#include "XapianIndex.h"
#include "DaemonState.h"
#include "OnDiskHandler.h"
#include "PinotSettings.h"
#include "PinotUtils.h"
#include "ServerThreads.h"

using namespace std;
using namespace Glib;

static double getFSFreeSpace(const string &path)
{
	double availableBlocks = 0.0;
	double blockSize = 0.0;
	int statSuccess = -1;
#ifdef HAVE_STATFS
	struct statfs fsStats;

	statSuccess = statfs(PinotSettings::getInstance().m_daemonIndexLocation.c_str(), &fsStats);
	availableBlocks = (uintmax_t)fsStats.f_bavail;
	blockSize = fsStats.f_bsize;
#else
#ifdef HAVE_STATVFS
	struct statvfs vfsStats;

	statSuccess = statvfs(path.c_str(), &vfsStats);
	availableBlocks = (uintmax_t)vfsStats.f_bavail;
	// f_frsize isn't supported by all implementations
	blockSize = (vfsStats.f_frsize ? vfsStats.f_frsize : vfsStats.f_bsize);
#endif
#endif
	// Did it fail ?
	if ((statSuccess == -1) ||
		(blockSize == 0.0))
	{
		return -1.0;
	}

	double mbRatio = blockSize / (1024 * 1024);
	double availableMbSize = availableBlocks * mbRatio;
#ifdef DEBUG
	cout << "DaemonState::getFSFreeSpace: " << availableBlocks << " blocks of " << blockSize
		<< " bytes (" << mbRatio << ")" << endl;
#endif

	return availableMbSize;
}

// A function object to stop DirectoryScanner threads with for_each()
struct StopScannerThreadFunc
{
public:
	void operator()(map<WorkerThread *, Thread *>::value_type &p)
	{
		string type(p.first->getType());

		if (type == "DirectoryScannerThread")
		{
			p.first->stop();
#ifdef DEBUG
			cout << "StopScannerThreadFunc: stopped thread " << p.first->getId() << endl;
#endif
		}
	}
};

DaemonState::DaemonState() :
	ThreadsManager(PinotSettings::getInstance().m_daemonIndexLocation, 20),
	m_fullScan(false),
	m_reload(false),
	m_pDiskMonitor(MonitorFactory::getMonitor()),
	m_pDiskHandler(NULL)
{
#ifdef CHECK_DISK_SPACE
	// Check every minute
	m_timeoutConnection = Glib::signal_timeout().connect(sigc::mem_fun(*this,
		&DaemonState::on_activity_timeout), 60000);
	// Check right now before doing anything else
	DaemonState::on_activity_timeout();
#endif
	m_onThreadEndSignal.connect(sigc::mem_fun(*this, &DaemonState::on_thread_end));
}

DaemonState::~DaemonState()
{
	if (m_pDiskMonitor != NULL)
	{
		delete m_pDiskMonitor;
	}
	// Don't delete m_pDiskHandler, threads may need it
	// Since DaemonState is destroyed when the program exits, it's okay
}

bool DaemonState::on_activity_timeout(void)
{
	if (m_timeoutConnection.blocked() == false)
	{
		double availableMbSize = getFSFreeSpace(PinotSettings::getInstance().m_daemonIndexLocation);
		if (availableMbSize >= 0)
		{
#ifdef DEBUG
			cout << "DaemonState::on_activity_timeout: " << availableMbSize << " Mb free for "
				<< PinotSettings::getInstance().m_daemonIndexLocation << endl;
#endif
			if (availableMbSize < PinotSettings::getInstance().m_minimumDiskSpace)
			{
				// Stop indexing
				m_stopIndexing = true;

				if (m_threads.empty() == false)
				{
					if (write_lock_threads() == true)
					{
						// Stop threads
						for_each(m_threads.begin(), m_threads.end(), StopScannerThreadFunc());
						m_threads.clear();

						unlock_threads();
					}
				}

				cerr << "Stopped indexing because of low disk space" << endl;
			}
			else if (m_stopIndexing == true)
			{
				// Go ahead
				m_stopIndexing = false;

				cerr << "Resumed indexing following low disk space condition" << endl;
			}
		}
	}

	return true;
}

bool DaemonState::crawl_location(const string &locationToCrawl, bool isSource, bool doMonitoring)
{
	DirectoryScannerThread *pScannerThread = NULL;

	if (m_stopIndexing == true)
	{
#ifdef DEBUG
		cout << "DaemonState::crawl_location: stopped indexing" << endl;
#endif
		return true;
	}

	if (locationToCrawl.empty() == true)
	{
		return false;
	}

	if (doMonitoring == false)
	{
		// Monitoring is not necessary, but we still have to pass the handler
		// so that we can act on documents that have been deleted
		pScannerThread = new DirectoryScannerThread(locationToCrawl, isSource, m_fullScan,
			NULL, m_pDiskHandler);
	}
	else
	{
		pScannerThread = new DirectoryScannerThread(locationToCrawl, isSource, m_fullScan,
			m_pDiskMonitor, m_pDiskHandler);
	}
	pScannerThread->getFileFoundSignal().connect(sigc::mem_fun(*this, &DaemonState::on_message_filefound));

	return start_thread(pScannerThread);
}

void DaemonState::start(bool forceFullScan)
{
	// Disable implicit flushing after a change
	WorkerThread::immediateFlush(false);

	// Do full scans ?
	if (forceFullScan == true)
	{
		m_fullScan = true;
	}
	else
	{
		Rand randomStuff;
		guint32 randomArray[5];

		randomStuff.set_seed(randomArray[2]);
		int randomNum = randomStuff.get_int_range(0, 10);
		if (randomNum >= 7)
		{
			m_fullScan = true;
		}
#ifdef DEBUG
		cout << "DaemonState::start: picked " << randomNum << endl;
#endif
	}

	// Fire up the disk monitor thread
	if (m_pDiskHandler == NULL)
	{
		m_pDiskHandler = new OnDiskHandler();
	}
	MonitorThread *pDiskMonitorThread = new MonitorThread(m_pDiskMonitor, m_pDiskHandler);
	pDiskMonitorThread->getDirectoryFoundSignal().connect(sigc::mem_fun(*this, &DaemonState::on_message_filefound));
	start_thread(pDiskMonitorThread, true);

	set<PinotSettings::IndexableLocation>::const_iterator locationIter = PinotSettings::getInstance().m_indexableLocations.begin();
	if (locationIter != PinotSettings::getInstance().m_indexableLocations.end())
	{
		// Crawl this now
		crawl_location(locationIter->m_name, true, locationIter->m_monitor);
	}
}

void DaemonState::reload(void)
{
	// Reload whenever possible
	m_reload = true;
}

void DaemonState::on_thread_end(WorkerThread *pThread)
{
	string indexedUrl;

	if (pThread == NULL)
	{
		return;
	}

	// What type of thread was it ?
	string type(pThread->getType());
#ifdef DEBUG
	cout << "DaemonState::on_thread_end: end of thread " << type << " " << pThread->getId() << endl;
#endif
	if (type == "DirectoryScannerThread")
	{
		DirectoryScannerThread *pScannerThread = dynamic_cast<DirectoryScannerThread *>(pThread);
		if (pScannerThread == NULL)
		{
			delete pThread;
			return;
		}

		string crawledLocation(pScannerThread->getDirectory());

		// Explicitely flush the index once a directory has been crawled
		XapianIndex index(PinotSettings::getInstance().m_daemonIndexLocation);
		index.flush();

#ifdef DEBUG
		cout << "DaemonState::on_thread_end: done crawling " << crawledLocation << endl;
#endif
		// Another location to crawl ?
		if (write_lock_lists() == true)
		{
			PinotSettings::IndexableLocation currentLocation;
			currentLocation.m_name = crawledLocation;

			set<PinotSettings::IndexableLocation>::const_iterator locationIter = PinotSettings::getInstance().m_indexableLocations.find(currentLocation);
			if (locationIter != PinotSettings::getInstance().m_indexableLocations.end())
			{
				// Get the next location
				++locationIter;
				if (locationIter != PinotSettings::getInstance().m_indexableLocations.end())
				{
					crawl_location(locationIter->m_name, true, locationIter->m_monitor);
				}
			}
#ifdef DEBUG
			else cout << "DaemonState::on_thread_end: nothing else to crawl" << endl;
#endif

			unlock_lists();
		}
#ifdef DEBUG
		else cout << "DaemonState::on_thread_end: nothing else to crawl" << endl;
#endif
	}
	else if (type == "IndexingThread")
	{
		IndexingThread *pIndexThread = dynamic_cast<IndexingThread *>(pThread);
		if (pIndexThread == NULL)
		{
			delete pThread;
			return;
		}

		// Get the URL we have just indexed
		indexedUrl = pIndexThread->getURL();
	}
	else if (type == "UnindexingThread")
	{
		// FIXME: anything to do ?
	}
	else if (type == "MonitorThread")
	{
		// FIXME: do something about this
	}
	else if (type == "DBusServletThread")
	{
		DBusServletThread *pDBusThread = dynamic_cast<DBusServletThread *>(pThread);
		if (pDBusThread == NULL)
		{
			delete pThread;
			return;
		}

		if (pDBusThread->mustQuit() == true)
		{
			// Disconnect the timeout signal
			if (m_timeoutConnection.connected() == true)
			{
				m_timeoutConnection.block();
				m_timeoutConnection.disconnect();
			}
			m_signalQuit(0);
		}
	}

	// Did it fail ?
	int errorNum = pThread->getErrorNum();
	if ((errorNum > 0) &&
		(indexedUrl.empty() == false))
	{
		CrawlHistory history(PinotSettings::getInstance().getHistoryDatabaseName());

		// An entry should already exist for this
		history.updateItem(indexedUrl, CrawlHistory::ERROR, time(NULL), errorNum);
	}

	// Delete the thread
	delete pThread;

	// Are we supposed to reload the configuration ?
	// Wait until there are no threads running (except background ones)
	if ((m_reload == true) &&
		(get_threads_count() == 0))
	{
#ifdef DEBUG
		cout << "DaemonState::on_thread_end: stopping all threads" << endl;
#endif
		// Stop background threads
		stop_threads();
		// ...clear the queues
		clear_queues();

		// Reload
		PinotSettings &settings = PinotSettings::getInstance();
		settings.clear();
		settings.loadGlobal(string(SYSCONFDIR) + "/pinot/globalconfig.xml");
		settings.load();

		// ...and restart everything 
		start(false);
#ifdef DEBUG
		cout << "DaemonState::on_thread_end: reloaded" << endl;
#endif

		m_reload = false;
	}
#ifdef DEBUG
	cout << "DaemonState::on_thread_end: reload status " << m_reload << endl;
#endif

	// We might be able to run a queued action
	pop_queue(indexedUrl);
}

void DaemonState::on_message_filefound(const DocumentInfo &docInfo, const string &sourceLabel, bool isDirectory)
{
	if (isDirectory == false)
	{
		DocumentInfo docCopy(docInfo);
		set<string> labels;

		// Insert a label that identifies the source
		labels.insert(sourceLabel);
		docCopy.setLabels(labels);

		queue_index(docCopy);
	}
	else
	{
		string location(docInfo.getLocation());

		crawl_location(location.substr(7), false, true);
#ifdef DEBUG
		cout << "DaemonState::on_message_filefound: new directory " << location.substr(7) << endl;
#endif
	}
}

sigc::signal1<void, int>& DaemonState::getQuitSignal(void)
{
	return m_signalQuit;
}


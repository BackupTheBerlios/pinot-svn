/*
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

#include <iostream>
#include <sigc++/class_slot.h>
#include <glibmm/ustring.h>
#include <glibmm/stringutils.h>
#include <glibmm/convert.h>
#include <glibmm/thread.h>

#include "Url.h"
#include "MonitorFactory.h"
#include "XapianIndex.h"
#include "DaemonState.h"
#include "MboxHandler.h"
#include "OnDiskHandler.h"
#include "PinotSettings.h"
#include "PinotUtils.h"
#include "WorkerThreads.h"

using namespace std;
using namespace Glib;

DaemonState::DaemonState() :
	ThreadsManager(PinotSettings::getInstance().m_daemonIndexLocation, 10),
	m_pMailMonitor(MonitorFactory::getMonitor()),
	m_pDiskMonitor(MonitorFactory::getMonitor()),
	m_crawling(false)
{
	m_onThreadEndSignal.connect(SigC::slot(*this, &DaemonState::on_thread_end));
}

DaemonState::~DaemonState()
{
	if (m_pDiskMonitor != NULL)
	{
		delete m_pDiskMonitor;
	}
	if (m_pMailMonitor != NULL)
	{
		delete m_pMailMonitor;
	}
}

void DaemonState::start(void)
{
	string locationToCrawl;

	// Disable implicit flushing after a change
	WorkerThread::immediateFlush(false);

	// Fire up the mail monitor thread now
	MboxHandler *pMbox = new MboxHandler();
	// Connect to its update signal
	pMbox->getUpdateSignal().connect(
		SigC::slot(*this, &DaemonState::on_message_indexupdate));
	MonitorThread *pMailMonitorThread = new MonitorThread(m_pMailMonitor, pMbox);
	pMailMonitorThread->getDirectoryFoundSignal().connect(SigC::slot(*this, &DaemonState::on_message_filefound));
	start_thread(pMailMonitorThread, true);

	// Same for the disk monitor thread
	OnDiskHandler *pDisk = new OnDiskHandler();
	// Connect to its update signal
	pDisk->getUpdateSignal().connect(
		SigC::slot(*this, &DaemonState::on_message_indexupdate));
	MonitorThread *pDiskMonitorThread = new MonitorThread(m_pDiskMonitor, pDisk);
	pDiskMonitorThread->getDirectoryFoundSignal().connect(SigC::slot(*this, &DaemonState::on_message_filefound));
	start_thread(pDiskMonitorThread, true);

	for (set<PinotSettings::TimestampedItem>::const_iterator locationIter = PinotSettings::getInstance().m_indexableLocations.begin();
		locationIter != PinotSettings::getInstance().m_indexableLocations.end(); ++locationIter)
	{
		if (locationToCrawl.empty() == true)
		{
			// Crawl this now
			locationToCrawl = locationIter->m_name;
		}
		else
		{
			// This will be crawled next
			m_crawlQueue.push(locationIter->m_name);
		}
	}

	// Anything to crawl before starting monitoring ?
	if (locationToCrawl.empty() == false)
	{
		// Scan the directory and import all its files
		DirectoryScannerThread *pScannerThread = new DirectoryScannerThread(m_pDiskMonitor,
			locationToCrawl, 0, true);
		pScannerThread->getFileFoundSignal().connect(SigC::slot(*this, &DaemonState::on_message_filefound));

		m_crawling = start_thread(pScannerThread);
	}
}

void DaemonState::on_thread_end(WorkerThread *pThread)
{
	string indexedUrl;

	if (pThread == NULL)
	{
		return;
	}
#ifdef DEBUG
	cout << "DaemonState::on_thread_end: end of thread " << pThread->getId() << endl;
#endif

	// What type of thread was it ?
	string type(pThread->getType());
	if (type == "DirectoryScannerThread")
	{
		DirectoryScannerThread *pScannerThread = dynamic_cast<DirectoryScannerThread *>(pThread);
		if (pScannerThread == NULL)
		{
			delete pThread;
			return;
		}

		DirectoryScannerThread *pNewScannerThread = NULL;

		// Another location to crawl ?
		if (write_lock_lists() == true)
		{
			if (m_crawlQueue.empty() == false)
			{
				string locationToCrawl(m_crawlQueue.front());

				set<PinotSettings::TimestampedItem> &indexableLocations = PinotSettings::getInstance().m_indexableLocations;
				for (set<PinotSettings::TimestampedItem>::iterator locationIter = indexableLocations.begin();
			                locationIter != indexableLocations.end(); ++locationIter)
				{
					if (locationIter->m_name == pScannerThread->getDirectory())
					{
						PinotSettings::TimestampedItem location(*locationIter);

						// Set the timestamp
						location.m_modTime = time(NULL);
						indexableLocations.erase(locationIter);
						indexableLocations.insert(location);

						break;
					}
				}

				pNewScannerThread = new DirectoryScannerThread(m_pDiskMonitor,
					locationToCrawl, 0, true);
				pNewScannerThread->getFileFoundSignal().connect(SigC::slot(*this,
					&DaemonState::on_message_filefound));

				m_crawlQueue.pop();
			}
			else
			{
				// Done with crawling
				m_crawling = false;
			}

			unlock_lists();
		}

		// Explicitely flush the index once a directory has been crawled
		XapianIndex index(PinotSettings::getInstance().m_daemonIndexLocation);
		index.flush();

		// Start a new scanner thread ?
		if (pNewScannerThread != NULL)
		{
			m_crawling = start_thread(pNewScannerThread);
		}
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
		UnindexingThread *pUnindexThread = dynamic_cast<UnindexingThread *>(pThread);
		if (pUnindexThread == NULL)
		{
			delete pThread;
			return;
		}

		// FIXME: anything to do ?
	}
	else if (type == "MonitorThread")
	{
		// FIXME: do something about this
	}

	// Delete the thread
	delete pThread;;

	// We might be able to run a queued action
	pop_queue(indexedUrl);
}

void DaemonState::on_message_indexupdate(IndexedDocument docInfo, unsigned int docId, string indexName)
{
	// FIXME: anything to do ?
}

void DaemonState::on_message_filefound(const string &location, const string &sourceLabel, bool isDirectory)
{
	if (isDirectory == false)
	{
		Url urlObj(location);
		DocumentInfo docInfo(urlObj.getFile(), location, "", "");
		set<string> labels;

		// Insert a label that identifies the source
		labels.insert(sourceLabel);
		docInfo.setLabels(labels);

		queue_index(docInfo);
	}
	else
	{
		DirectoryScannerThread *pScannerThread = new DirectoryScannerThread(m_pDiskMonitor,
			location.substr(7), 0, true);
		pScannerThread->getFileFoundSignal().connect(SigC::slot(*this, &DaemonState::on_message_filefound));

#ifdef DEBUG
		cout << "DaemonState::on_message_filefound: new directory " << location.substr(7) << endl;
#endif
		m_crawling = start_thread(pScannerThread);
	}
}


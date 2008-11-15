/*
 *  Copyright 2005-2008 Fabrice Colin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <errno.h>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glibmm/miscutils.h>

#include "config.h"
#include "NLS.h"
#include "MIMEScanner.h"
#include "TimeConverter.h"
#include "Timer.h"
#include "Url.h"
#include "CrawlHistory.h"
#include "MetaDataBackup.h"
#include "DBusIndex.h"
#include "ModuleFactory.h"
#include "DaemonState.h"
#include "PinotSettings.h"
#include "ServerThreads.h"

using namespace Glib;
using namespace std;

static void updateLabels(unsigned int docId, MetaDataBackup &metaData,
	IndexInterface *pIndex, set<string> &labels, gboolean resetLabels)
{
	DocumentInfo docInfo;

	if (pIndex == NULL)
	{
		return;
	}

	// If it's a reset, remove labels from the metadata backup
	if ((resetLabels == TRUE) &&
		(pIndex->getDocumentInfo(docId, docInfo) == true))
	{
		metaData.deleteItem(docInfo, DocumentInfo::SERIAL_LABELS);
	}

	// Get the current labels 
	if (resetLabels == TRUE)
	{
		labels.clear();
		pIndex->getDocumentLabels(docId, labels);
	}
	docInfo.setLabels(labels);
	metaData.addItem(docInfo, DocumentInfo::SERIAL_LABELS);
}

static ustring g_xmlDescription;

static bool loadXMLDescription(void)
{
	bool readFile = false;

	if (g_xmlDescription.empty() == false)
	{
		return true;
	}

	ifstream xmlFile;
	string xmlFileName(PREFIX);

	xmlFileName += "/share/pinot/pinot-dbus-daemon.xml";
	xmlFile.open(xmlFileName.c_str());
	if (xmlFile.good() == true)
	{
		xmlFile.seekg(0, ios::end);
		int length = xmlFile.tellg();
		xmlFile.seekg(0, ios::beg);

		char *pXmlBuffer = new char[length + 1];
		xmlFile.read(pXmlBuffer, length);
		if (xmlFile.fail() == false)
		{
			pXmlBuffer[length] = '\0';
			g_xmlDescription = pXmlBuffer;
			readFile = true;
		}
		delete[] pXmlBuffer;
	}
	xmlFile.close();

	if (readFile == false)
	{
		cerr << "File " << xmlFileName << " couldn't be read" << endl;
	}
	return readFile;
}

DirectoryScannerThread::DirectoryScannerThread(const string &dirName, bool isSource,
	bool fullScan, bool isReindex,
	MonitorInterface *pMonitor, MonitorHandler *pHandler,
	unsigned int maxLevel, bool followSymLinks) :
	IndexingThread(),
	m_dirName(dirName),
	m_fullScan(fullScan),
	m_isReindex(isReindex),
	m_pMonitor(pMonitor),
	m_pHandler(pHandler),
	m_sourceId(0),
	m_currentLevel(0),
	m_maxLevel(maxLevel),
	m_followSymLinks(followSymLinks),
	m_delegateIndexing(false)
{
	// This is not set in the configuration file
	char *pEnvVar = getenv("PINOT_DELEGATE_INDEXING");
	if ((pEnvVar != NULL) &&
		(strlen(pEnvVar) > 0) &&
		(strncasecmp(pEnvVar, "Y", 1) == 0))
	{
		m_delegateIndexing = true;
	}

	if (m_dirName.empty() == false)
	{
		CrawlHistory crawlHistory(PinotSettings::getInstance().getHistoryDatabaseName());

		if (isSource == true)
		{
			// Does this source exist ?
			if (crawlHistory.hasSource("file://" + m_dirName, m_sourceId) == false)
			{
				// Create it
				m_sourceId = crawlHistory.insertSource("file://" + m_dirName);
			}
		}
		else
		{
			map<unsigned int, string> fileSources;

			// What source does this belong to ?
			for(map<unsigned int, string>::const_iterator sourceIter = fileSources.begin();
				sourceIter != fileSources.end(); ++sourceIter)
			{
				if (sourceIter->second.length() < m_dirName.length())
				{
					// Skip
					continue;
				}

				if (sourceIter->second.substr(0, m_dirName.length()) == m_dirName)
				{
					// That's the one
					m_sourceId = sourceIter->first;
					break;
				}
			}
		}
	}
}

DirectoryScannerThread::~DirectoryScannerThread()
{
}

string DirectoryScannerThread::getType(void) const
{
	return "DirectoryScannerThread";
}

string DirectoryScannerThread::getDirectory(void) const
{
	return m_dirName;
}

void DirectoryScannerThread::stop(void)
{
	// Disconnect the signal
	sigc::signal2<void, DocumentInfo, bool>::slot_list_type slotsList = m_signalFileFound.slots();
	sigc::signal2<void, DocumentInfo, bool>::slot_list_type::iterator slotIter = slotsList.begin();
	if (slotIter != slotsList.end())
	{
		if (slotIter->empty() == false)
		{
			slotIter->block();
			slotIter->disconnect();
		}
	}
	WorkerThread::stop();
}

sigc::signal2<void, DocumentInfo, bool>& DirectoryScannerThread::getFileFoundSignal(void)
{
	return m_signalFileFound;
}

void DirectoryScannerThread::cacheUpdate(const string &location, time_t mTime,
	CrawlHistory &crawlHistory)
{
	m_updateCache[location] = mTime;

	if (m_updateCache.size() > 500)
	{
		flushUpdates(crawlHistory);
	}
}

void DirectoryScannerThread::flushUpdates(CrawlHistory &crawlHistory)
{
#ifdef DEBUG
	cout << "DirectoryScannerThread::flushUpdates: flushing updates" << endl;
#endif

	// Update these records
	crawlHistory.updateItems(m_updateCache, CrawlHistory::CRAWLED);
	m_updateCache.clear();

#ifdef DEBUG
	cout << "DirectoryScannerThread::flushUpdates: flushed updates" << endl;
#endif
}

void DirectoryScannerThread::foundFile(const DocumentInfo &docInfo)
{
	if ((docInfo.getLocation().empty() == true) ||
		(m_done == true))
	{
		return;
	}

	if (m_delegateIndexing == false)
	{
		// Reset base class members
		m_docInfo = docInfo;
		m_docId = 0;
		m_indexLocation = PinotSettings::getInstance().m_daemonIndexLocation;
		m_update = false;

		IndexingThread::doWork();
#ifdef DEBUG
		cout << "DirectoryScannerThread::foundFile: indexed " << docInfo.getLocation() << " to " << m_docId << endl;
#endif
	}
	else
	{
		// Delegate indexing
		m_signalFileFound(docInfo, false);
	}
}

bool DirectoryScannerThread::scanEntry(const string &entryName, CrawlHistory &crawlHistory)
{
	CrawlHistory::CrawlStatus status = CrawlHistory::UNKNOWN;
	string location("file://" + entryName);
	time_t itemDate;
	struct stat fileStat;
	int entryStatus = 0;
	bool scanSuccess = true, reportFile = false, itemExists = false;

	if (entryName.empty() == true)
	{
#ifdef DEBUG
		cout << "DirectoryScannerThread::scanEntry: no name" << endl;
#endif
		return false;
	}

	// Skip . .. and dotfiles
	Url urlObj(location);
	if (urlObj.getFile()[0] == '.')
	{
#ifdef DEBUG
		cout << "DirectoryScannerThread::scanEntry: skipped dotfile " << urlObj.getFile() << endl;
#endif
		return false;
	}

	// Is this item in the database already ?
	itemExists = crawlHistory.hasItem(location, status, itemDate);

	if (m_followSymLinks == false)
	{
		entryStatus = lstat(entryName.c_str(), &fileStat);
	}
	else
	{
		// Stat the files pointed to by symlinks
		entryStatus = stat(entryName.c_str(), &fileStat);
	}

	if (entryStatus == -1)
	{
		entryStatus = errno;
		scanSuccess = false;
#ifdef DEBUG
		cout << "DirectoryScannerThread::scanEntry: stat failed with error " << entryStatus << endl;
#endif
	}
	// Is it a file or a directory ?
	else if (S_ISLNK(fileStat.st_mode))
	{
		// This won't happen when m_followSymLinks is true
#ifdef DEBUG
		cout << "DirectoryScannerThread::scanEntry: skipped symlink" << endl;
#endif
		return false;
	}
	else if (S_ISREG(fileStat.st_mode))
	{
		// Is this file blacklisted ?
		// We have to check early so that if necessary the file's status stays at CRAWLING 
		// and it is removed from the index at the end of this crawl
		if (PinotSettings::getInstance().isBlackListed(entryName) == false)
		{
			reportFile = true;
		}
	}
	else if (S_ISDIR(fileStat.st_mode))
	{
		// Can we scan this directory ?
		if (((m_maxLevel == 0) ||
			(m_currentLevel < m_maxLevel)) &&
			(PinotSettings::getInstance().isBlackListed(entryName) == false))
		{
			++m_currentLevel;

			// Open the directory
			DIR *pDir = opendir(entryName.c_str());
			if (pDir != NULL)
			{
#ifdef DEBUG
				cout << "DirectoryScannerThread::scanEntry: entering " << entryName << endl;
#endif
				if (m_pMonitor != NULL)
				{
					// Monitor first so that we don't miss events
					// If monitoring is not possible, record the first case
					if ((m_pMonitor->addLocation(entryName, true) == false) &&
						(entryStatus != MONITORING_FAILED))
					{
						entryStatus = MONITORING_FAILED;
					}
				}

				// Iterate through this directory's entries
				struct dirent *pDirEntry = readdir(pDir);
				while ((m_done == false) &&
					(pDirEntry != NULL))
				{
					char *pEntryName = pDirEntry->d_name;

					// Skip . .. and dotfiles
					if ((pEntryName != NULL) &&
						(pEntryName[0] != '.'))
					{
						string subEntryName(entryName);

						if (entryName[entryName.length() - 1] != '/')
						{
							subEntryName += "/";
						}
						subEntryName += pEntryName;

						// Scan this entry
						if (scanEntry(subEntryName, crawlHistory) == false)
						{
#ifdef DEBUG
							cout << "DirectoryScannerThread::scanEntry: failed to open "
								<< subEntryName << endl;
#endif
						}
					}

					// Next entry
					pDirEntry = readdir(pDir);
				}
#ifdef DEBUG
				cout << "DirectoryScannerThread::scanEntry: done with " << entryName << endl;
#endif

				// Close the directory
				closedir(pDir);
				--m_currentLevel;
				reportFile = true;
			}
			else
			{
				entryStatus = errno;
				scanSuccess = false;
#ifdef DEBUG
				cout << "DirectoryScannerThread::scanEntry: opendir failed with error " << entryStatus << endl;
#endif
			}
		}
	}
	else
	{
#ifdef DEBUG
		cout << "DirectoryScannerThread::scanEntry: unknown entry type" << endl;
#endif
		entryStatus = ENOENT;
		scanSuccess = false;
	}

	// If a major error occured, this won't be true
	if (reportFile == true)
	{
		if (itemExists == false)
		{
			// Record it
			crawlHistory.insertItem(location, CrawlHistory::CRAWLED, m_sourceId, fileStat.st_mtime);
			itemExists = true;
#ifdef DEBUG
			cout << "DirectoryScannerThread::scanEntry: reporting new file " << entryName << endl;
#endif
		}
		else
		{
			// Was it modified after the last crawl ?
			if (itemDate >= fileStat.st_mtime)
			{
				// No, it wasn't
				reportFile = false;
			}

			// History of modified files, especially the timestamp, is always updated
			// Others' are updated only if we are doing a full scan because
			// the status has to be reset to CRAWLED, so that they are not unindexed
			if ((reportFile == true) ||
				(m_fullScan == true))
			{
#ifdef DEBUG
				cout << "DirectoryScannerThread::scanEntry: updating " << entryName << endl;
#endif
				cacheUpdate(location, fileStat.st_mtime, crawlHistory);
			}

		}

		if (reportFile == true)
		{
			DocumentInfo docInfo("", location, "", "");
			set<string> labels;
			stringstream labelStream;

			if (S_ISDIR(fileStat.st_mode))
			{
				docInfo.setType("x-directory/normal");
			}
			else
			{
				// We might as well scan the file for its type now
				docInfo.setType(MIMEScanner::scanFile(entryName));
			}
			docInfo.setTimestamp(TimeConverter::toTimestamp(fileStat.st_mtime));
			docInfo.setSize(fileStat.st_size);
			// Insert a label that identifies the source
			labelStream << "X-SOURCE" << m_sourceId;
			labels.insert(labelStream.str());
			docInfo.setLabels(labels);

			foundFile(docInfo);
		}
	}

	// Did an error occur ?
	if (entryStatus != 0)
	{
		time_t timeNow = time(NULL);

		// Record this error
		if (itemExists == false)
		{
			crawlHistory.insertItem(location, CrawlHistory::ERROR, m_sourceId, timeNow, entryStatus);
		}
		else
		{
			crawlHistory.updateItem(location, CrawlHistory::ERROR, timeNow, entryStatus);
		}
	}

	return scanSuccess;
}

void DirectoryScannerThread::doWork(void)
{
	CrawlHistory crawlHistory(PinotSettings::getInstance().getHistoryDatabaseName());
	MetaDataBackup metaData(PinotSettings::getInstance().getHistoryDatabaseName());
	Timer scanTimer;
	set<string> urls;
	unsigned int currentOffset = 0;

	if (m_dirName.empty() == true)
	{
		return;
	}
	scanTimer.start();

	if (m_fullScan == true)
	{
		cout << "Doing a full scan on " << m_dirName << endl;

		// Update this source's items status so that we can detect files that have been deleted
		crawlHistory.updateItemsStatus(CrawlHistory::CRAWLED, CrawlHistory::CRAWLING,
			m_sourceId);
	}
	// Remove errors
	crawlHistory.deleteItems(m_sourceId, CrawlHistory::ERROR);

	if (scanEntry(m_dirName, crawlHistory) == false)
	{
		m_errorNum = OPENDIR_FAILED;
		m_errorParam = m_dirName;
	}
	flushUpdates(crawlHistory);
	cout << "Scanned " << m_dirName << " in " << scanTimer.stop() << " ms" << endl;

	if (m_done == true)
	{
#ifdef DEBUG
		cout << "DirectoryScannerThread::doWork: leaving cleanup until next crawl" << endl;
#endif
		return;
	}

	if (m_fullScan == true)
	{
		scanTimer.start();

		// All files left with status CRAWLING were not found in this crawl
		// Chances are they were removed after the last full scan
		while ((m_pHandler != NULL) &&
			(crawlHistory.getSourceItems(m_sourceId, CrawlHistory::CRAWLING, urls,
				currentOffset, currentOffset + 100) > 0))
		{
			for (set<string>::const_iterator urlIter = urls.begin();
				urlIter != urls.end(); ++urlIter)
			{
				// Inform the MonitorHandler
				if (m_pHandler->fileDeleted(urlIter->substr(7)) == true)
				{
					// Delete this item
					crawlHistory.deleteItem(*urlIter);
					metaData.deleteItem(DocumentInfo("", *urlIter, "", ""), DocumentInfo::SERIAL_ALL);
				}
			}

			// Next
			if (urls.size() < 100)
			{
				break;
			}
			currentOffset += 100;
		}
		cout << "Cleaned up " << currentOffset + urls.size()
			<< " history entries in " << scanTimer.stop() << " ms" << endl;
	}

	if (m_isReindex == true)
	{
		urls.clear();
		currentOffset = 0;
		scanTimer.start();

		IndexInterface *pIndex = PinotSettings::getInstance().getIndex(PinotSettings::getInstance().m_daemonIndexLocation);
		// Restore user-set metadata, if any
		while ((pIndex != NULL) &&
			(pIndex->isGood() == true) &&
			(metaData.getItems("file://", urls,
				currentOffset, currentOffset + 100) == true))
		{
			for (set<string>::const_iterator urlIter = urls.begin();
				urlIter != urls.end(); ++urlIter)
			{
				unsigned int docId = pIndex->hasDocument(*urlIter);

				if (docId > 0)
				{
					DocumentInfo docInfo("", *urlIter, "", "");

					if (metaData.getItem(docInfo, DocumentInfo::SERIAL_ALL) == true)
					{
						pIndex->updateDocumentInfo(docId, docInfo);
						pIndex->setDocumentLabels(docId, docInfo.getLabels(), true);
					}
				}
			}

			// Next
			if (urls.size() < 100)
			{
				break;
			}
			currentOffset += 100;
		}
		cout << "Restored user-set metadata for " << currentOffset + urls.size()
			<< " documents in " << scanTimer.stop() << " ms" << endl;
	}
}

DBusServletThread::DBusServletThread(DaemonState *pServer, DBusServletInfo *pInfo) :
	WorkerThread(),
	m_pServer(pServer),
	m_pServletInfo(pInfo),
	m_mustQuit(false)
{
}

DBusServletThread::~DBusServletThread()
{
}

string DBusServletThread::getType(void) const
{
	return "DBusServletThread";
}

DBusServletInfo *DBusServletThread::getServletInfo(void) const
{
	return m_pServletInfo;
}

bool DBusServletThread::mustQuit(void) const
{
	return m_mustQuit;
}

void DBusServletThread::doWork(void)
{
	PinotSettings &settings = PinotSettings::getInstance();
	IndexInterface *pIndex = settings.getIndex(settings.m_daemonIndexLocation);
	MetaDataBackup metaData(settings.getHistoryDatabaseName());
	DBusError error;
	bool processedMessage = true, updateLabelsCache = false, flushIndex = false;

	if ((m_pServer == NULL) ||
		(m_pServletInfo == NULL) ||
		(pIndex == NULL))
	{
		return;
	}

	dbus_error_init(&error);

	// Access the settings' labels list directly
	set<string> &labelsCache = settings.m_labels;
	if (labelsCache.empty() == true)
	{
		pIndex->getLabels(labelsCache);
	}

#ifdef DEBUG
	const char *pSender = dbus_message_get_sender(m_pServletInfo->m_pRequest);
	if (pSender != NULL)
	{
		cout << "DBusServletThread::doWork: called by " << pSender << endl;
	}
	else
	{
		cout << "DBusServletThread::doWork: called by unknown sender" << endl;
	}
#endif

	if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "de.berlios.Pinot", "GetStatistics") == TRUE)
	{
		CrawlHistory crawlHistory(settings.getHistoryDatabaseName());
		unsigned int crawledFilesCount = crawlHistory.getItemsCount(CrawlHistory::CRAWLED);
		unsigned int docsCount = pIndex->getDocumentsCount();
		gboolean lowDiskSpace = FALSE, onBattery = FALSE, crawling = FALSE;

#ifdef DEBUG
		cout << "DBusServletThread::doWork: received GetStatistics" << endl;
#endif
		// Prepare the reply
		if (m_pServletInfo->newReply() == true)
		{
			if (m_pServer->is_flag_set(DaemonState::LOW_DISK_SPACE) == true)
			{
				lowDiskSpace = TRUE;
			}
			if (m_pServer->is_flag_set(DaemonState::ON_BATTERY) == true)
			{
				onBattery = TRUE;
			}
			if (m_pServer->is_flag_set(DaemonState::CRAWLING) == true)
			{
				crawling = TRUE;
			}
#ifdef DEBUG
			cout << "DBusServletThread::doWork: replying with " << crawledFilesCount
				<< " " << docsCount << " " << lowDiskSpace << onBattery << crawling << endl;
#endif

			dbus_message_append_args(m_pServletInfo->m_pReply,
				DBUS_TYPE_UINT32, &crawledFilesCount,
				DBUS_TYPE_UINT32, &docsCount,
				DBUS_TYPE_BOOLEAN, &lowDiskSpace,
				DBUS_TYPE_BOOLEAN, &onBattery,
				DBUS_TYPE_BOOLEAN, &crawling,
				DBUS_TYPE_INVALID);
		}
	}
	else if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "de.berlios.Pinot", "Reload") == TRUE)
	{
		if (dbus_message_get_args(m_pServletInfo->m_pRequest, &error,
			DBUS_TYPE_INVALID) == TRUE)
		{
			gboolean reloading = TRUE;

#ifdef DEBUG
			cout << "DBusServletThread::doWork: received Reload" << endl;
#endif
			m_pServer->reload();

			// Prepare the reply
			if (m_pServletInfo->newReply() == true)
			{
				dbus_message_append_args(m_pServletInfo->m_pReply,
					DBUS_TYPE_BOOLEAN, &reloading,
					DBUS_TYPE_INVALID);
			}
		}
	}
	else if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "de.berlios.Pinot", "Stop") == TRUE)
	{
		if (dbus_message_get_args(m_pServletInfo->m_pRequest, &error,
			DBUS_TYPE_INVALID) == TRUE)
		{
			int exitStatus = EXIT_SUCCESS;

#ifdef DEBUG
			cout << "DBusServletThread::doWork: received Stop" << endl;
#endif
			m_pServer->set_flag(DaemonState::STOPPED);

			// Prepare the reply
			if (m_pServletInfo->newReply() == true)
			{
				dbus_message_append_args(m_pServletInfo->m_pReply,
					DBUS_TYPE_INT32, &exitStatus,
					DBUS_TYPE_INVALID);
			}

			m_mustQuit = true;
		}
	}
	else if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "de.berlios.Pinot", "HasDocument") == TRUE)
	{
		char *pUrl = NULL;
		unsigned int docId = 0;

		if (dbus_message_get_args(m_pServletInfo->m_pRequest, &error,
			DBUS_TYPE_STRING, &pUrl,
			DBUS_TYPE_INVALID) == TRUE)
		{
#ifdef DEBUG
			cout << "DBusServletThread::doWork: received HasDocument " << pUrl << endl;
#endif
			if (pUrl != NULL)
			{
				string url(pUrl);

				// Check the index
				docId = pIndex->hasDocument(url);
			}

			// Prepare the reply
			if (m_pServletInfo->newReply() == true)
			{
				dbus_message_append_args(m_pServletInfo->m_pReply,
					DBUS_TYPE_UINT32, &docId,
					DBUS_TYPE_INVALID);
			}
		}
	}
	else if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "de.berlios.Pinot", "GetLabels") == TRUE)
	{
#ifdef DEBUG
		cout << "DBusServletThread::doWork: received GetLabels" << endl;
#endif
		// This method doesn't take any argument
		m_pServletInfo->m_pArray = g_ptr_array_new();

		for (set<string>::const_iterator labelIter = labelsCache.begin();
			labelIter != labelsCache.end(); ++labelIter)
		{
			string labelName(*labelIter);

			g_ptr_array_add(m_pServletInfo->m_pArray, const_cast<char*>(labelName.c_str()));
		}

		// Prepare the reply
		m_pServletInfo->newReplyWithArray();
	}
	else if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "de.berlios.Pinot", "AddLabel") == TRUE)
	{
		char *pLabel = NULL;

		if (dbus_message_get_args(m_pServletInfo->m_pRequest, &error,
			DBUS_TYPE_STRING, &pLabel,
			DBUS_TYPE_INVALID) == TRUE)
		{
#ifdef DEBUG
			cout << "DBusServletThread::doWork: received AddLabel " << pLabel << endl;
#endif
			if (pLabel != NULL)
			{
				string labelName(pLabel);

				// Add the label
				flushIndex = pIndex->addLabel(labelName);
				// Is this a known label ?
				if (labelsCache.find(labelName) == labelsCache.end())
				{
					// No, it isn't but that's okay
					labelsCache.insert(labelName);
					updateLabelsCache = true;
				}
			}

			// Prepare the reply
			if (m_pServletInfo->newReply() == true)
			{
				dbus_message_append_args(m_pServletInfo->m_pReply,
					DBUS_TYPE_STRING, &pLabel,
					DBUS_TYPE_INVALID);
			}
		}
	}
	else if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "de.berlios.Pinot", "RenameLabel") == TRUE)
	{
		char *pOldLabel = NULL;
		char *pNewLabel = NULL;

		if (dbus_message_get_args(m_pServletInfo->m_pRequest, &error,
			DBUS_TYPE_STRING, &pOldLabel,
			DBUS_TYPE_STRING, &pNewLabel,
			DBUS_TYPE_INVALID) == TRUE)
		{
			// Nothing to do, this was obsoleted
#ifdef DEBUG
			cout << "DBusServletThread::doWork: received RenameLabel " << pOldLabel << ", " << pNewLabel << endl;
#endif
			
			// Prepare the reply
			if (m_pServletInfo->newReply() == true)
			{
				dbus_message_append_args(m_pServletInfo->m_pReply,
					DBUS_TYPE_STRING, &pNewLabel,
					DBUS_TYPE_INVALID);
			}
		}
	}
	else if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "de.berlios.Pinot", "DeleteLabel") == TRUE)
	{
		char *pLabel = NULL;

		if (dbus_message_get_args(m_pServletInfo->m_pRequest, &error,
			DBUS_TYPE_STRING, &pLabel,
			DBUS_TYPE_INVALID) == TRUE)
		{
#ifdef DEBUG
			cout << "DBusServletThread::doWork: received DeleteLabel " << pLabel << endl;
#endif
			if (pLabel != NULL)
			{
				// Delete the label
				flushIndex = pIndex->deleteLabel(pLabel);
				// Update the labels list
				set<string>::const_iterator labelIter = labelsCache.find(pLabel);
				if (labelIter != labelsCache.end())
				{
					labelsCache.erase(labelIter);
					updateLabelsCache = true;
				}

				// Update the metadata backup
				metaData.deleteLabel(pLabel);
			}

			// Prepare the reply
			if (m_pServletInfo->newReply() == true)
			{
				dbus_message_append_args(m_pServletInfo->m_pReply,
					DBUS_TYPE_STRING, &pLabel,
					DBUS_TYPE_INVALID);
			}
		}
	}
	else if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "de.berlios.Pinot", "GetDocumentLabels") == TRUE)
	{
		unsigned int docId = 0;

		if (dbus_message_get_args(m_pServletInfo->m_pRequest, &error,
			DBUS_TYPE_UINT32, &docId,
			DBUS_TYPE_INVALID) == TRUE)
		{
			set<string> labels;

#ifdef DEBUG
			cout << "DBusServletThread::doWork: received GetDocumentLabels " << docId << endl;
#endif
			if (pIndex->getDocumentLabels(docId, labels) == true)
			{
				m_pServletInfo->m_pArray = g_ptr_array_new();

				for (set<string>::const_iterator labelIter = labels.begin();
					labelIter != labels.end(); ++labelIter)
				{
					string labelName(*labelIter);

					g_ptr_array_add(m_pServletInfo->m_pArray, const_cast<char*>(labelName.c_str()));
				}

				// Prepare the reply
				m_pServletInfo->newReplyWithArray();
			}
			else
			{
				m_pServletInfo->newErrorReply("de.berlios.Pinot.GetDocumentLabels",
					" failed");
			}
		}
	}
	else if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "de.berlios.Pinot", "SetDocumentLabels") == TRUE)
	{
		char **ppLabels = NULL;
		dbus_uint32_t labelsCount = 0;
		unsigned int docId = 0;
		gboolean resetLabels = TRUE;

		if (dbus_message_get_args(m_pServletInfo->m_pRequest, &error,
			DBUS_TYPE_UINT32, &docId,
			DBUS_TYPE_ARRAY, DBUS_TYPE_STRING, &ppLabels, &labelsCount,
			DBUS_TYPE_BOOLEAN, &resetLabels,
			DBUS_TYPE_INVALID) == TRUE)
		{
			set<string> labels;

			for (dbus_uint32_t labelIndex = 0; labelIndex < labelsCount; ++labelIndex)
			{
				if (ppLabels[labelIndex] == NULL)
				{
					break;
				}

				string labelName(ppLabels[labelIndex]);
				labels.insert(labelName);
				// Is this a known label ?
				if (labelsCache.find(labelName) == labelsCache.end())
				{
					// No, it isn't but that's okay
					labelsCache.insert(labelName);
					updateLabelsCache = true;
				}
			}
#ifdef DEBUG
			cout << "DBusServletThread::doWork: received SetDocumentLabels on ID " << docId
				<< ", " << labelsCount << " labels" << ", " << resetLabels << endl;
#endif

			// Set labels
			flushIndex = pIndex->setDocumentLabels(docId, labels, ((resetLabels == TRUE) ? true : false));

			// Update the metadata backup
			updateLabels(docId, metaData, pIndex, labels, resetLabels);

			// Free container types
			g_strfreev(ppLabels);

			// Prepare the reply
			if (m_pServletInfo->newReply() == true)
			{
				dbus_message_append_args(m_pServletInfo->m_pReply,
					DBUS_TYPE_UINT32, &docId,
					DBUS_TYPE_INVALID);
			}
		}
	}
	else if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "de.berlios.Pinot", "SetDocumentsLabels") == TRUE)
	{
		char **ppDocIds = NULL;
		char **ppLabels = NULL;
		dbus_uint32_t idsCount = 0;
		dbus_uint32_t labelsCount = 0;
		gboolean resetLabels = TRUE;

		if (dbus_message_get_args(m_pServletInfo->m_pRequest, &error,
			DBUS_TYPE_ARRAY, DBUS_TYPE_STRING, &ppDocIds, &idsCount,
			DBUS_TYPE_ARRAY, DBUS_TYPE_STRING, &ppLabels, &labelsCount,
			DBUS_TYPE_BOOLEAN, &resetLabels,
			DBUS_TYPE_INVALID) == TRUE)
		{
			set<unsigned int> docIds;
			set<string> labels;

			for (dbus_uint32_t idIndex = 0; idIndex < idsCount; ++idIndex)
			{
				if (ppDocIds[idIndex] == NULL)
				{
					break;
				}

				docIds.insert((unsigned int)atoi(ppDocIds[idIndex]));
			}
			for (dbus_uint32_t labelIndex = 0; labelIndex < labelsCount; ++labelIndex)
			{
				if (ppLabels[labelIndex] == NULL)
				{
					break;
				}

				string labelName(ppLabels[labelIndex]);
				labels.insert(labelName);
				// Is this a known label ?
				if (labelsCache.find(labelName) == labelsCache.end())
				{
					// No, it isn't but that's okay
					labelsCache.insert(labelName);
					updateLabelsCache = true;
				}
			}
#ifdef DEBUG
			cout << "DBusServletThread::doWork: received SetDocumentsLabels on " << docIds.size()
				<< " IDs, " << labelsCount << " labels" << ", " << resetLabels << endl;
#endif
			// Set labels
			if (pIndex->setDocumentsLabels(docIds, labels, ((resetLabels == TRUE) ? true : false)) == true)
			{
				resetLabels = TRUE;
				flushIndex = true;
			}

			// Update the metadata backup
			for (set<unsigned int>::const_iterator docIter = docIds.begin();
				docIter != docIds.end(); ++docIter)
			{
				updateLabels(*docIter, metaData, pIndex, labels, resetLabels);
			}

			// Free container types
			g_strfreev(ppDocIds);
			g_strfreev(ppLabels);

			// Prepare the reply
			if (m_pServletInfo->newReply() == true)
			{
				dbus_message_append_args(m_pServletInfo->m_pReply,
					DBUS_TYPE_BOOLEAN, &resetLabels,
					DBUS_TYPE_INVALID);
			}
		}
	}
	else if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "de.berlios.Pinot", "GetDocumentInfo") == TRUE)
	{
		unsigned int docId = 0;

		if (dbus_message_get_args(m_pServletInfo->m_pRequest, &error,
			DBUS_TYPE_UINT32, &docId,
			DBUS_TYPE_INVALID) == TRUE)
		{
			DocumentInfo docInfo;

#ifdef DEBUG
			cout << "DBusServletThread::doWork: received GetDocumentInfo on " << docId << endl;
#endif
			if (pIndex->getDocumentInfo(docId, docInfo) == true)
			{
				// Prepare the reply
				if (m_pServletInfo->newReply() == true)
				{
					DBusMessageIter iter;

					dbus_message_iter_init_append(m_pServletInfo->m_pReply, &iter);
					if (DBusIndex::documentInfoToDBus(&iter, 0, docInfo) == false)
					{
						dbus_message_unref(m_pServletInfo->m_pReply);
						m_pServletInfo->newErrorReply("de.berlios.Pinot.GetDocumentInfo",
							"Unknown error");
					}
				}
			}
			else
			{
				m_pServletInfo->newErrorReply("de.berlios.Pinot.GetDocumentInfo",
					"Unknown document");
			}
		}
	}
	else if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "de.berlios.Pinot", "SetDocumentInfo") == TRUE)
	{
		DBusMessageIter iter;
		DocumentInfo docInfo;
		unsigned int docId = 0;

		dbus_message_iter_init(m_pServletInfo->m_pRequest, &iter);
		if (DBusIndex::documentInfoFromDBus(&iter, docId, docInfo) == false)
		{
			m_pServletInfo->newErrorReply("de.berlios.Pinot.SetDocumentInfo",
				"Unknown error");
		}
		else
		{
#ifdef DEBUG
			cout << "DBusServletThread::doWork: received SetDocumentInfo on " << docId << endl;
#endif

			// Update the document info
			flushIndex = pIndex->updateDocumentInfo(docId, docInfo);

			// Update the metadata backup
			metaData.addItem(docInfo, DocumentInfo::SERIAL_FIELDS);

			// Prepare the reply
			if (m_pServletInfo->newReply() == true)
			{
				dbus_message_append_args(m_pServletInfo->m_pReply,
					DBUS_TYPE_UINT32, &docId,
					DBUS_TYPE_INVALID);
			}
		}
	}
	else if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "de.berlios.Pinot", "Query") == TRUE)
	{
		char *pSearchText = NULL;
		char *pEngineType = NULL;
		char *pEngineOption = NULL;
		dbus_uint32_t startDoc = 0, maxHits = 0;

		if (dbus_message_get_args(m_pServletInfo->m_pRequest, &error,
			DBUS_TYPE_STRING, &pEngineType,
			DBUS_TYPE_STRING, &pEngineOption,
			DBUS_TYPE_STRING, &pSearchText,
			DBUS_TYPE_UINT32, &startDoc,
			DBUS_TYPE_UINT32, &maxHits,
			DBUS_TYPE_INVALID) == TRUE)
		{
			bool replyWithError = true;

#ifdef DEBUG
			cout << "DBusServletThread::doWork: received Query " << pSearchText << ", " << startDoc << "/" << maxHits << endl;
#endif
			if (pSearchText != NULL)
			{
				stringstream queryNameStr;

				// Give the query a unique name
				queryNameStr << "DBUS" << m_id;
				m_pServletInfo->m_queryName = queryNameStr.str();
				m_pServletInfo->m_simpleQuery = false;

				QueryProperties queryProps(m_pServletInfo->m_queryName, pSearchText);
				queryProps.setMaximumResultsCount(maxHits);

				string engineType, engineOption;

				// Provide reasonable defaults 
				if (((pEngineType == NULL) || (strlen(pEngineType) == 0)) &&
					((pEngineOption == NULL) || (strlen(pEngineOption) == 0)))
				{
					engineType = settings.m_defaultBackend;
					engineOption = settings.m_daemonIndexLocation;
				}
				else
				{
					engineType = pEngineType;
					engineOption = pEngineOption;
				}

				m_pServletInfo->m_pThread = new EngineQueryThread(engineType,
					engineType, engineOption, queryProps, startDoc);
			}

			if (replyWithError == true)
			{
				m_pServletInfo->newErrorReply("de.berlios.Pinot.SimpleQuery",
					"Query failed");
			}
		}
	}
	// FIXME: this method will soon be obsoleted
	else if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "de.berlios.Pinot", "SimpleQuery") == TRUE)
	{
		char *pSearchText = NULL;
		dbus_uint32_t maxHits = 0;

		if (dbus_message_get_args(m_pServletInfo->m_pRequest, &error,
			DBUS_TYPE_STRING, &pSearchText,
			DBUS_TYPE_UINT32, &maxHits,
			DBUS_TYPE_INVALID) == TRUE)
		{
			bool replyWithError = true;

#ifdef DEBUG
			cout << "DBusServletThread::doWork: received SimpleQuery " << pSearchText << ", " << maxHits << endl;
#endif
			if (pSearchText != NULL)
			{
				stringstream queryNameStr;

				// Give the query a unique name
				queryNameStr << "DBUS" << m_id;
				m_pServletInfo->m_queryName = queryNameStr.str();
				m_pServletInfo->m_simpleQuery = true;

				QueryProperties queryProps(m_pServletInfo->m_queryName, pSearchText);
				queryProps.setMaximumResultsCount(maxHits);

				m_pServletInfo->m_pThread = new EngineQueryThread(settings.m_defaultBackend,
					settings.m_defaultBackend, settings.m_daemonIndexLocation,
					queryProps, 0);
			}

			if (replyWithError == true)
			{
				m_pServletInfo->newErrorReply("de.berlios.Pinot.SimpleQuery",
					"Query failed");
			}
		}
	}
	else if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "de.berlios.Pinot", "UpdateDocument") == TRUE)
	{
		unsigned int docId = 0;

		if (dbus_message_get_args(m_pServletInfo->m_pRequest, &error,
			DBUS_TYPE_UINT32, &docId,
			DBUS_TYPE_INVALID) == TRUE)
		{
			DocumentInfo docInfo;

#ifdef DEBUG
			cout << "DBusServletThread::doWork: received UpdateDocument " << docId << endl;
#endif
			if (pIndex->getDocumentInfo(docId, docInfo) == true)
			{
				// Update document
				m_pServer->queue_index(docInfo);
			}

			// Prepare the reply
			if (m_pServletInfo->newReply() == true)
			{
				dbus_message_append_args(m_pServletInfo->m_pReply,
					DBUS_TYPE_UINT32, &docId,
					DBUS_TYPE_INVALID);
			}
		}
	}
	else if (dbus_message_is_method_call(m_pServletInfo->m_pRequest, "org.freedesktop.DBus.Introspectable", "Introspect") == TRUE)
	{
#ifdef DEBUG
		cout << "DBusServletThread::doWork: received Introspect" << endl;
#endif
		if (loadXMLDescription() == true)
		{
			// Prepare the reply
			if (m_pServletInfo->newReply() == true)
			{
				const char *pXmlData = g_xmlDescription.c_str();

				dbus_message_append_args(m_pServletInfo->m_pReply,
					DBUS_TYPE_STRING, &pXmlData,
					DBUS_TYPE_INVALID);
			}
		}
	}
	else
	{
#ifdef DEBUG
		cout << "DBusServletThread::doWork: foreign message for/from " << dbus_message_get_interface(m_pServletInfo->m_pRequest)
			<< " " << dbus_message_get_member(m_pServletInfo->m_pRequest) << endl;
#endif
		processedMessage = false;
	}

	// Did an error occur ?
	if (error.message != NULL)
	{
#ifdef DEBUG
		cout << "DBusServletThread::doWork: error occured: " << error.message << endl;
#endif
		// Use the error message as reply
		m_pServletInfo->newErrorReply(error.name, error.message);
	}

	dbus_error_free(&error);

	// Set labels ?
	if ((updateLabelsCache == true) &&
		(pIndex->setLabels(labelsCache, false) == false))
	{
		// Updating failed... reset the cache
		labelsCache.clear();
		pIndex->getLabels(labelsCache);
#ifdef DEBUG
		cout << "DBusServletThread::doWork: failed to update labels" << endl;
#endif
	}

	// Flush the index ?
	if (flushIndex == true)
	{
		// Flush now for the sake of the client application
		pIndex->flush();
	}

	delete pIndex;
}


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

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <exception>
#include <iostream>
#include <sigc++/class_slot.h>
#include <sigc++/compatibility.h>
#include <sigc++/slot.h>
#include <glibmm/miscutils.h>

#include "Languages.h"
#include "TimeConverter.h"
#include "Url.h"
#include "CrawlHistory.h"
#include "XapianIndex.h"
#include "XapianEngine.h"
#include "config.h"
#include "NLS.h"
#include "DaemonState.h"
#include "PinotSettings.h"
#include "ServerThreads.h"

using namespace SigC;
using namespace Glib;
using namespace std;

static DBusMessage *newDBusReply(DBusMessage *pMessage)
{
        if (pMessage == NULL) 
        {
                return NULL;
        }

        DBusMessage *pReply = dbus_message_new_method_return(pMessage);
        if (pReply != NULL)
        {
                return pReply;
        }

        return NULL;
}

MonitorThread::MonitorThread(MonitorInterface *pMonitor, MonitorHandler *pHandler) :
	WorkerThread(),
	m_ctrlReadPipe(-1),
	m_ctrlWritePipe(-1),
	m_pMonitor(pMonitor),
	m_pHandler(pHandler)
{
	int pipeFds[2];

	if (pipe(pipeFds) == 0)
	{
		// This pipe will allow to stop select()
		m_ctrlReadPipe = pipeFds[0];
		m_ctrlWritePipe = pipeFds[1];
	}
}

MonitorThread::~MonitorThread()
{
	close(m_ctrlReadPipe);
	close(m_ctrlWritePipe);
}

string MonitorThread::getType(void) const
{
	return "MonitorThread";
}

bool MonitorThread::stop(void)
{
	// Disconnect the signal
	Signal3<void, const DocumentInfo&, const std::string&, bool>::slot_list_type slotsList = m_signalDirectoryFound.slots();
	Signal3<void, const DocumentInfo&, const std::string&, bool>::slot_list_type::iterator slotIter = slotsList.begin();
	if (slotIter != slotsList.end())
	{
		if (slotIter->empty() == false)
		{
			slotIter->block();
			slotIter->disconnect();
		}
	}
	m_done = true;
	write(m_ctrlWritePipe, "X", 1);

	return true;
}

Signal3<void, const DocumentInfo&, const std::string&, bool>& MonitorThread::getDirectoryFoundSignal(void)
{
	return m_signalDirectoryFound;
}

void MonitorThread::processEvents(void)
{
	CrawlHistory history(PinotSettings::getInstance().m_historyDatabase);
	queue<MonitorEvent> events;

#ifdef DEBUG
	cout << "MonitorThread::processEvents: checking for events" << endl;
#endif
	if ((m_pMonitor == NULL) ||
		(m_pMonitor->retrievePendingEvents(events) == false))
	{
#ifdef DEBUG
		cout << "MonitorThread::processEvents: failed to retrieve pending events" << endl;
#endif
		return;
	}
#ifdef DEBUG
	cout << "MonitorThread::processEvents: retrieved " << events.size() << " events" << endl;
#endif

	while ((events.empty() == false) &&
		(m_done == false))
	{
		MonitorEvent &event = events.front();

		if ((event.m_location.empty() == true) ||
			(event.m_type == MonitorEvent::UNKNOWN))
		{
			// Next
			events.pop();
			continue;
		}
#ifdef DEBUG
		cout << "MonitorThread::processEvents: event " << event.m_type << " on "
			<< event.m_location << " " << event.m_isDirectory << endl;
#endif

		// What's the event code ?
		if (event.m_type == MonitorEvent::EXISTS)
		{
			if (event.m_isDirectory == false)
			{
				m_pHandler->fileExists(event.m_location);
			}
		}
		else if (event.m_type == MonitorEvent::CREATED)
		{
			if (event.m_isDirectory == false)
			{
				m_pHandler->fileCreated(event.m_location);
			}
			else
			{
				DocumentInfo docInfo("", string("file://") + event.m_location, "", "");

				// Report this directory so that it is crawled
				m_signalDirectoryFound(docInfo, "", true);
			}
		}
		else if (event.m_type == MonitorEvent::WRITE_CLOSED)
		{
			if (event.m_isDirectory == false)
			{
				CrawlHistory::CrawlStatus status = CrawlHistory::UNKNOWN;
				struct stat fileStat;
				time_t itemDate;

				if (history.hasItem("file://" + event.m_location, status, itemDate) == true)
				{
					// Was the file actually modified ?
					if ((stat(event.m_location.c_str(), &fileStat) == 0) &&
						(itemDate < fileStat.st_mtime))
					{
						m_pHandler->fileModified(event.m_location);
					}
#ifdef DEBUG
					else cout << "MonitorThread::processEvents: file wasn't modified" << endl;
#endif
				}
#ifdef DEBUG
				else cout << "MonitorThread::processEvents: file wasn't crawled" << endl;
#endif
			}
		}
		else if (event.m_type == MonitorEvent::MOVED)
		{
			if (event.m_isDirectory == false)
			{
				m_pHandler->fileMoved(event.m_location, event.m_previousLocation);
			}
			else
			{
				// We should receive this only if the destination directory is monitored too
				m_pHandler->directoryMoved(event.m_location, event.m_previousLocation);
			}
		}
		else if (event.m_type == MonitorEvent::DELETED)
		{
			if (event.m_isDirectory == false)
			{
				m_pHandler->fileDeleted(event.m_location);
			}
			else
			{
				// The monitor should have stopped monitoring this
				// In practice, events for the files in this directory will already have been received 
				m_pHandler->directoryDeleted(event.m_location);
			}
		}

		// Next
		events.pop();
	}
}

void MonitorThread::doWork(void)
{
	if ((m_pHandler == NULL) ||
		(m_pMonitor == NULL))
	{
		m_status = _("No monitoring handler");
		return;
	}

	// Initialize the handler
	m_pHandler->initialize();

	// Get the list of files to monitor
	const set<string> &fileNames = m_pHandler->getFileNames();
	for (set<string>::const_iterator fileIter = fileNames.begin();
		fileIter != fileNames.end(); ++fileIter)
	{
		m_pMonitor->addLocation(*fileIter, false);
	}
	// Directories, if any, are set elsewhere
	// In the case of OnDiskHandler, they are set by DirectoryScannerThread

	// There might already be events that need processing
	processEvents();

	// Wait for something to happen
	while (m_done == false)
	{
		struct timeval selectTimeout;
		fd_set listenSet;

		selectTimeout.tv_sec = 60;
		selectTimeout.tv_usec = 0;

		FD_ZERO(&listenSet);
		if (m_ctrlReadPipe >= 0)
		{
			FD_SET(m_ctrlReadPipe, &listenSet);
		}

		m_pHandler->flushIndex();

		// The file descriptor may change over time
		int monitorFd = m_pMonitor->getFileDescriptor();
		FD_SET(monitorFd, &listenSet);
		if (monitorFd < 0)
		{
			m_status = _("Couldn't initialize file monitor");
			return;
		}

		int fdCount = select(max(monitorFd, m_ctrlReadPipe) + 1, &listenSet, NULL, NULL, &selectTimeout);
		if ((fdCount < 0) &&
			(errno != EINTR))
		{
#ifdef DEBUG
			cout << "MonitorThread::doWork: select() failed" << endl;
#endif
			break;
		}
		else if (FD_ISSET(monitorFd, &listenSet))
		{
			processEvents();
		}
	}
}

DirectoryScannerThread::DirectoryScannerThread(const string &dirName, bool isSource,
	unsigned int maxLevel, bool followSymLinks,
	MonitorInterface *pMonitor, MonitorHandler *pHandler) :
	WorkerThread(),
	m_dirName(dirName),
	m_maxLevel(maxLevel),
	m_followSymLinks(followSymLinks),
	m_pMonitor(pMonitor),
	m_pHandler(pHandler),
	m_currentLevel(0),
	m_sourceId(0)
{
	if (m_dirName.empty() == false)
	{
		CrawlHistory history(PinotSettings::getInstance().m_historyDatabase);

		if (isSource == true)
		{
			// Does this source exist ?
			if (history.hasSource("file://" + m_dirName, m_sourceId) == false)
			{
				// Create it
				m_sourceId = history.insertSource("file://" + m_dirName);
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

bool DirectoryScannerThread::stop(void)
{
	// Disconnect the signal
	Signal3<void, const DocumentInfo&, const std::string&, bool>::slot_list_type slotsList = m_signalFileFound.slots();
	Signal3<void, const DocumentInfo&, const std::string&, bool>::slot_list_type::iterator slotIter = slotsList.begin();
	if (slotIter != slotsList.end())
	{
		if (slotIter->empty() == false)
		{
			slotIter->block();
			slotIter->disconnect();
		}
	}
	m_done = true;

	return true;
}

Signal3<void, const DocumentInfo&, const std::string&, bool>& DirectoryScannerThread::getFileFoundSignal(void)
{
	return m_signalFileFound;
}

void DirectoryScannerThread::foundFile(const DocumentInfo &docInfo)
{
	char labelStr[64];

	if ((docInfo.getLocation().empty() == true) ||
		(m_done == true))
	{
		return;
	}

	// This identifies the source
	snprintf(labelStr, 64, "SOURCE%u", m_sourceId);

	m_signalFileFound(docInfo, labelStr, false);
}

bool DirectoryScannerThread::scanEntry(const string &entryName)
{
	CrawlHistory history(PinotSettings::getInstance().m_historyDatabase);
	CrawlHistory::CrawlStatus status = CrawlHistory::UNKNOWN;
	time_t itemDate;
	struct stat fileStat;
	int statSuccess = 0;

	if (entryName.empty() == true)
	{
		return false;
	}

	// Skip . .. and dotfiles
	Url urlObj("file://" + entryName);
	if (urlObj.getFile()[0] == '.')
	{
		return false;
	}

	if (m_followSymLinks == false)
	{
		statSuccess = lstat(entryName.c_str(), &fileStat);
	}
	else
	{
		// Stat the files pointed to by symlinks
		statSuccess = stat(entryName.c_str(), &fileStat);
	}

	if (statSuccess == -1)
	{
		return false;
	}

	bool itemExists = history.hasItem("file://" + entryName, status, itemDate);

	// Is it a file or a directory ?
	if (S_ISLNK(fileStat.st_mode))
	{
		// This won't happen when m_followSymLinks is true
		return false;
	}
	else if (S_ISREG(fileStat.st_mode))
	{
		DocumentInfo docInfo;
		bool reportFile = false;

		docInfo.setLocation("file://" + entryName);

		// Is this file blacklisted ?
		// We have to check early so that if necessary the file's status stays at CRAWLING 
		// and it is removed from the index at the end of this crawl
		if (PinotSettings::getInstance().isBlackListed(entryName) == false)
		{
			if (itemExists == false)
			{
				// Record it
				history.insertItem(docInfo.getLocation(), CrawlHistory::CRAWLED, m_sourceId, fileStat.st_mtime);
#ifdef DEBUG
				cout << "DirectoryScannerThread::scanEntry: reporting new file " << entryName << endl;
#endif
				reportFile = true;
			}
			else
			{
				// Update the record
				history.updateItem(docInfo.getLocation(), CrawlHistory::CRAWLED, fileStat.st_mtime);

				// Was it last crawled after it was modified ?
				if (itemDate < fileStat.st_mtime)
				{
#ifdef DEBUG
					cout << "DirectoryScannerThread::scanEntry: reporting modified file " << entryName << endl;
#endif
					// No, crawl and index it again
					reportFile = true;
				}
			}
		}

		if (reportFile == true)
		{
			Url urlObj(docInfo.getLocation());

			docInfo.setTitle(urlObj.getFile());
			docInfo.setTimestamp(TimeConverter::toTimestamp(fileStat.st_mtime));
			docInfo.setSize(fileStat.st_size);

			foundFile(docInfo);
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
			if (pDir == NULL)
			{
				return false;
			}
#ifdef DEBUG
			cout << "DirectoryScannerThread::scanEntry: entering " << entryName << endl;
#endif

			if (m_pMonitor != NULL)
			{
				// Monitor first so that we don't miss events
				m_pMonitor->addLocation(entryName, true);
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
					if (scanEntry(subEntryName) == false)
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
		}
	}
	else
	{
		return false;
	}

	return true;
}

void DirectoryScannerThread::doWork(void)
{
	CrawlHistory history(PinotSettings::getInstance().m_historyDatabase);
	set<string> deletedFiles;

	if (m_dirName.empty() == true)
	{
		return;
	}

	// Update this source's items status
	history.updateItemsStatus(m_sourceId, CrawlHistory::CRAWLED, CrawlHistory::CRAWLING);

	if (scanEntry(m_dirName) == false)
	{
		m_status = _("Couldn't open directory");
		m_status += " ";
		m_status += m_dirName;
	}

	if (m_done == true)
	{
#ifdef DEBUG
		cout << "DirectoryScannerThread::doWork: leaving cleanup until next crawl" << endl;
#endif
		return;
	}

	// All files with status set to CRAWLING were not found in this crawl
	// Chances are they were removed after the previous crawl
	if ((m_pHandler != NULL) &&
		(history.getSourceItems(m_sourceId, CrawlHistory::CRAWLING, deletedFiles) > 0))
	{
#ifdef DEBUG
		cout << "DirectoryScannerThread::doWork: " << deletedFiles.size() << " files were deleted" << endl;
#endif
		for(set<string>::const_iterator fileIter = deletedFiles.begin();
			fileIter != deletedFiles.end(); ++fileIter)
		{
			// Inform the MonitorHandler
			if (m_pHandler->fileDeleted(fileIter->substr(7)) == true)
			{
				// Delete this item
				history.deleteItem(*fileIter);
			}
		}
	}
#ifdef DEBUG
	cout << "DirectoryScannerThread::doWork: done crawling " << m_dirName << endl;
#endif
}

DBusServletThread::DBusServletThread(DaemonState *pServer, DBusConnection *pConnection, DBusMessage *pRequest) :
	WorkerThread(),
	m_pServer(pServer),
	m_pConnection(pConnection),
	m_pRequest(pRequest),
	m_pReply(NULL),
	m_pArray(NULL),
	m_mustQuit(false)
{
}

DBusServletThread::~DBusServletThread()
{
	if (m_pArray != NULL)
	{
		// Free the array
		g_ptr_array_free(m_pArray, TRUE);
	}
	if (m_pRequest != NULL)
	{
		dbus_message_ref(m_pRequest);
	}
	if (m_pConnection != NULL)
	{
		dbus_connection_ref(m_pConnection);
	}
}

string DBusServletThread::getType(void) const
{
	return "DBusServletThread";
}

bool DBusServletThread::stop(void)
{
	m_done = true;
	return true;
}

DBusConnection *DBusServletThread::getConnection(void) const
{
	return m_pConnection;
}

DBusMessage *DBusServletThread::getReply(void) const
{
	return m_pReply;
}

bool DBusServletThread::mustQuit(void) const
{
	return m_mustQuit;
}

void DBusServletThread::doWork(void)
{
	XapianIndex index(PinotSettings::getInstance().m_daemonIndexLocation);
	DBusError error;
	const char *pSender = dbus_message_get_sender(m_pRequest);
	bool processedMessage = true, flushIndex = false;

	if ((m_pServer == NULL) ||
		(m_pConnection == NULL) ||
		(m_pRequest == NULL))
	{
		return;
	}

	dbus_error_init(&error);

#ifdef DEBUG
	if (pSender != NULL)
	{
		cout << "DBusServletThread::doWork: called by " << pSender << endl;
	}
	else
	{
		cout << "DBusServletThread::doWork: called by unknown sender" << endl;
	}
#endif

	if (dbus_message_is_method_call(m_pRequest, "de.berlios.Pinot", "DeleteLabel") == TRUE)
	{
		char *pLabel = NULL;

		if (dbus_message_get_args(m_pRequest, &error,
			DBUS_TYPE_STRING, &pLabel,
			DBUS_TYPE_INVALID) == TRUE)
		{
#ifdef DEBUG
			cout << "DBusServletThread::doWork: received DeleteLabel " << pLabel << endl;
#endif
			// Delete the label
			flushIndex = index.deleteLabel(pLabel);

			// Prepare the reply
			m_pReply = newDBusReply(m_pRequest);
			if (m_pReply != NULL)
			{
				dbus_message_append_args(m_pReply,
					DBUS_TYPE_STRING, &pLabel,
					DBUS_TYPE_INVALID);
			}
		}
	}
	else if (dbus_message_is_method_call(m_pRequest, "de.berlios.Pinot", "GetDocumentInfo") == TRUE)
	{
		unsigned int docId = 0;

		if (dbus_message_get_args(m_pRequest, &error,
			DBUS_TYPE_UINT32, &docId,
			DBUS_TYPE_INVALID) == TRUE)
		{
			DocumentInfo docInfo;

#ifdef DEBUG
			cout << "DBusServletThread::doWork: received GetDocumentInfo " << docId << endl;
#endif
			if (index.getDocumentInfo(docId, docInfo) == true)
			{
				// Prepare the reply
				m_pReply = newDBusReply(m_pRequest);
				if (m_pReply != NULL)
				{
					string language(Languages::toEnglish(docInfo.getLanguage()));
					const char *pTitle = docInfo.getTitle().c_str();
					const char *pLocation = docInfo.getLocation().c_str();
					const char *pType = docInfo.getType().c_str();
					const char *pLanguage = language.c_str();

					dbus_message_append_args(m_pReply,
						DBUS_TYPE_STRING, &pTitle,
						DBUS_TYPE_STRING, &pLocation,
						DBUS_TYPE_STRING, &pType,
						DBUS_TYPE_STRING, &pLanguage,
						DBUS_TYPE_INVALID);
				}
			}
			else
			{
				m_pReply = dbus_message_new_error(m_pRequest,
					"de.berlios.Pinot.GetDocumentInfo",
					"Unknown document");
			}
		}
	}
	else if (dbus_message_is_method_call(m_pRequest, "de.berlios.Pinot", "GetDocumentLabels") == TRUE)
	{
		unsigned int docId = 0;

		if (dbus_message_get_args(m_pRequest, &error,
			DBUS_TYPE_UINT32, &docId,
			DBUS_TYPE_INVALID) == TRUE)
		{
			set<string> labels;

#ifdef DEBUG
			cout << "DBusServletThread::doWork: received GetDocumentLabels " << docId << endl;
#endif
			if (index.getDocumentLabels(docId, labels) == true)
			{
				m_pArray = g_ptr_array_new();

				for (set<string>::const_iterator labelIter = labels.begin();
					labelIter != labels.end(); ++labelIter)
				{
					string labelName(*labelIter);

					g_ptr_array_add(m_pArray, const_cast<char*>(labelName.c_str()));
#ifdef DEBUG
					cout << "DBusServletThread::doWork: adding label " << m_pArray->len << " " << labelName << endl;
#endif
				}

				// Prepare the reply
				m_pReply = newDBusReply(m_pRequest);
				if (m_pReply != NULL)
				{
					dbus_message_append_args(m_pReply,
						DBUS_TYPE_ARRAY, DBUS_TYPE_STRING, &m_pArray->pdata, m_pArray->len,
						DBUS_TYPE_INVALID);
				}
			}
			else
			{
				m_pReply = dbus_message_new_error(m_pRequest,
					"de.berlios.Pinot.GetDocumentLabels",
					" failed");
			}
		}
	}
	else if (dbus_message_is_method_call(m_pRequest, "de.berlios.Pinot", "GetStatistics") == TRUE)
	{
		CrawlHistory history(PinotSettings::getInstance().m_historyDatabase);
		unsigned int crawledFilesCount = history.getItemsCount();
		unsigned int docsCount = index.getDocumentsCount();

#ifdef DEBUG
		cout << "DBusServletThread::doWork: received GetStatistics" << endl;
#endif
		// Prepare the reply
		m_pReply = newDBusReply(m_pRequest);
		if (m_pReply != NULL)
		{
			dbus_message_append_args(m_pReply,
				DBUS_TYPE_UINT32, &crawledFilesCount,
				DBUS_TYPE_UINT32, &docsCount,
				DBUS_TYPE_INVALID);
		}
	}
	else if (dbus_message_is_method_call(m_pRequest, "de.berlios.Pinot", "RenameLabel") == TRUE)
	{
		char *pOldLabel = NULL;
		char *pNewLabel = NULL;

		if (dbus_message_get_args(m_pRequest, &error,
			DBUS_TYPE_STRING, &pOldLabel,
			DBUS_TYPE_STRING, &pNewLabel,
			DBUS_TYPE_INVALID) == TRUE)
		{
#ifdef DEBUG
			cout << "DBusServletThread::doWork: received RenameLabel " << pOldLabel << ", " << pNewLabel << endl;
#endif
			// Rename the label
			flushIndex = index.renameLabel(pOldLabel, pNewLabel);

			// Prepare the reply
			m_pReply = newDBusReply(m_pRequest);
			if (m_pReply != NULL)
			{
				dbus_message_append_args(m_pReply,
					DBUS_TYPE_STRING, &pNewLabel,
					DBUS_TYPE_INVALID);
			}
		}
	}
	else if (dbus_message_is_method_call(m_pRequest, "de.berlios.Pinot", "SetDocumentInfo") == TRUE)
	{
		char *pTitle = NULL;
		char *pLocation = NULL;
		char *pType = NULL;
		char *pLanguage = NULL;
		unsigned int docId = 0;

		if (dbus_message_get_args(m_pRequest, &error,
			DBUS_TYPE_UINT32, &docId,
			DBUS_TYPE_STRING, &pTitle,
			DBUS_TYPE_STRING, &pLocation,
			DBUS_TYPE_STRING, &pType,
			DBUS_TYPE_STRING, &pLanguage,
			DBUS_TYPE_INVALID) == TRUE)
		{
			DocumentInfo docInfo(pTitle, pLocation, pType,
				((pLanguage != NULL) ? Languages::toLocale(pLanguage) : ""));

#ifdef DEBUG
			cout << "DBusServletThread::doWork: received SetDocumentInfo " << docId << ", " << pTitle
				<< ", " << pLocation << ", " << pType << ", " << pLanguage << endl;
#endif

			// Update the document info
			flushIndex = index.updateDocumentInfo(docId, docInfo);

			// Prepare the reply
			m_pReply = newDBusReply(m_pRequest);
			if (m_pReply != NULL)
			{
				dbus_message_append_args(m_pReply,
					DBUS_TYPE_UINT32, &docId,
					DBUS_TYPE_INVALID);
			}
		}
	}
	else if (dbus_message_is_method_call(m_pRequest, "de.berlios.Pinot", "SetDocumentLabels") == TRUE)
	{
		char **ppLabels = NULL;
		dbus_uint32_t labelsCount = 0;
		unsigned int docId = 0;
		gboolean resetLabels = TRUE;

		if (dbus_message_get_args(m_pRequest, &error,
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
				labels.insert(ppLabels[labelIndex]);
			}
#ifdef DEBUG
			cout << "DBusServletThread::doWork: received SetDocumentLabels on ID " << docId
				<< ", " << labelsCount << " labels" << ", " << resetLabels << endl;
#endif
			// Set labels
			flushIndex = index.setDocumentLabels(docId, labels, ((resetLabels == TRUE) ? true : false));

			// Free container types
			g_strfreev(ppLabels);

			// Prepare the reply
			m_pReply = newDBusReply(m_pRequest);
			if (m_pReply != NULL)
			{
				dbus_message_append_args(m_pReply,
					DBUS_TYPE_UINT32, &docId,
					DBUS_TYPE_INVALID);
			}
		}
	}
	else if (dbus_message_is_method_call(m_pRequest, "de.berlios.Pinot", "SetDocumentsLabels") == TRUE)
	{
		char **ppDocIds = NULL;
		char **ppLabels = NULL;
		dbus_uint32_t idsCount = 0;
		dbus_uint32_t labelsCount = 0;
		gboolean resetLabels = TRUE;

		if (dbus_message_get_args(m_pRequest, &error,
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
				labels.insert(ppLabels[labelIndex]);
			}
#ifdef DEBUG
			cout << "DBusServletThread::doWork: received SetDocumentLabels on " << docIds.size()
				<< " IDs, " << labelsCount << " labels" << ", " << resetLabels << endl;
#endif
			// Set labels
			flushIndex = index.setDocumentsLabels(docIds, labels, ((resetLabels == TRUE) ? true : false));

			// Free container types
			g_strfreev(ppDocIds);
			g_strfreev(ppLabels);

			// Prepare the reply
			m_pReply = newDBusReply(m_pRequest);
			if (m_pReply != NULL)
			{
				dbus_message_append_args(m_pReply,
					DBUS_TYPE_BOOLEAN, &flushIndex,
					DBUS_TYPE_INVALID);
			}
		}
	}
	else if (dbus_message_is_method_call(m_pRequest, "de.berlios.Pinot", "SimpleQuery") == TRUE)
	{
		char *pSearchText = NULL;
		dbus_uint32_t maxHits = 0;

		if (dbus_message_get_args(m_pRequest, &error,
			DBUS_TYPE_STRING, &pSearchText,
			DBUS_TYPE_UINT32, &maxHits,
			DBUS_TYPE_INVALID) == TRUE)
		{
			XapianEngine engine(PinotSettings::getInstance().m_daemonIndexLocation);
			bool replyWithError = true;

#ifdef DEBUG
			cout << "DBusServletThread::doWork: received SimpleQuery " << pSearchText << ", " << maxHits << endl;
#endif
			if (pSearchText != NULL)
			{
				QueryProperties queryProps("DBUS", pSearchText);

				// Run the query
				engine.setMaxResultsCount(maxHits);
				if (engine.runQuery(queryProps) == true)
				{
					const vector<Result> &resultsList = engine.getResults();
					vector<string> docIds;
					m_pArray = g_ptr_array_new();

					for (vector<Result>::const_iterator resultIter = resultsList.begin();
						resultIter != resultsList.end(); ++resultIter)
					{
						// We only need the document ID
						unsigned int docId = index.hasDocument(resultIter->getLocation());
						if (docId > 0)
						{
							char docIdStr[64];
							snprintf(docIdStr, 64, "%u", docId);
							docIds.push_back(docIdStr);
						}
					}

					for (vector<string>::const_iterator docIter = docIds.begin();
						docIter != docIds.end(); ++docIter)
					{
#ifdef DEBUG
						cout << "DBusServletThread::doWork: adding result " << m_pArray->len << " " << *docIter << endl;
#endif
						g_ptr_array_add(m_pArray, const_cast<char*>(docIter->c_str()));
					}

					// Prepare the reply
					m_pReply = newDBusReply(m_pRequest);
					if (m_pReply != NULL)
					{
						dbus_message_append_args(m_pReply,
							DBUS_TYPE_ARRAY, DBUS_TYPE_STRING, &m_pArray->pdata, m_pArray->len,
							DBUS_TYPE_INVALID);

						replyWithError = false;
					}
				}
			}

			if (replyWithError == true)
			{
				m_pReply = dbus_message_new_error(m_pRequest,
					"de.berlios.Pinot.SimpleQuery",
					"Query failed");
			}
		}
	}
	else if (dbus_message_is_method_call(m_pRequest, "de.berlios.Pinot", "Stop") == TRUE)
	{
		if (dbus_message_get_args(m_pRequest, &error,
			DBUS_TYPE_INVALID) == TRUE)
		{
			int exitStatus = EXIT_SUCCESS;

#ifdef DEBUG
			cout << "DBusServletThread::doWork: received Stop" << endl;
#endif
			// Prepare the reply
			m_pReply = newDBusReply(m_pRequest);
			if (m_pReply != NULL)
			{
				dbus_message_append_args(m_pReply,
					DBUS_TYPE_INT32, &exitStatus,
					DBUS_TYPE_INVALID);
			}

			m_mustQuit = true;
		}
	}
	else if (dbus_message_is_method_call(m_pRequest, "de.berlios.Pinot", "UpdateDocument") == TRUE)
	{
		unsigned int docId = 0;

		if (dbus_message_get_args(m_pRequest, &error,
			DBUS_TYPE_UINT32, &docId,
			DBUS_TYPE_INVALID) == TRUE)
		{
			DocumentInfo docInfo;

#ifdef DEBUG
			cout << "DBusServletThread::doWork: received UpdateDocument " << docId << endl;
#endif
			if (index.getDocumentInfo(docId, docInfo) == true)
			{
				// Update document
				m_pServer->queue_index(docInfo);
			}

			// Prepare the reply
			m_pReply = newDBusReply(m_pRequest);
			if (m_pReply != NULL)
			{
				dbus_message_append_args(m_pReply,
					DBUS_TYPE_UINT32, &docId,
					DBUS_TYPE_INVALID);
			}
		}
	}
	else
	{
#ifdef DEBUG
		cout << "DBusServletThread::doWork: foreign message for/from " << dbus_message_get_interface(m_pRequest)
			<< " " << dbus_message_get_member(m_pRequest) << endl;
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
		m_pReply = dbus_message_new_error(m_pRequest, error.name, error.message);
	}

	dbus_error_free(&error);

	if (flushIndex == true)
	{
		// Flush now for the sake of the client application
		index.flush();
	}

	// Send a reply ?
	if ((m_pConnection != NULL) &&
		(m_pReply != NULL))
	{
		dbus_connection_send(m_pConnection, m_pReply, NULL);
		dbus_connection_flush(m_pConnection);
#ifdef DEBUG
		cout << "DBusServletThread::doWork: sent reply" << endl;
#endif
		dbus_message_unref(m_pReply);
	}
}


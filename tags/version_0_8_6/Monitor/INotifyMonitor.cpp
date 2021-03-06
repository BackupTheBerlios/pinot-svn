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
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "config.h"
#include <sys/ioctl.h>
#ifdef HAVE_LINUX_INOTIFY_H
#include <stdint.h>
#include <linux/inotify.h>
#include "linux-inotify-syscalls.h"
#else
#ifdef HAVE_SYS_INOTIFY_H
#include <sys/inotify.h>
#endif
#endif
#include <string.h>
#include <errno.h>
#include <iostream>
#include <set>

#include "INotifyMonitor.h"

using std::string;
using std::map;
using std::set;
using std::queue;
using std::pair;
using std::cout;
using std::cerr;
using std::endl;

INotifyMonitor::INotifyMonitor() :
	MonitorInterface(),
	m_noWatchesLeft(false)
{
	pthread_mutex_init(&m_mutex, NULL);
	m_monitorFd = inotify_init();
	if (m_monitorFd < 0)
	{
		char errBuffer[1024];

		strerror_r(errno, errBuffer, 1024);
		cerr << "Couldn't initialize inotify: " << errBuffer << endl;
	}
}

INotifyMonitor::~INotifyMonitor()
{
	if (m_monitorFd >= 0)
	{
		close(m_monitorFd);
	}
	pthread_mutex_destroy(&m_mutex);
}

bool INotifyMonitor::removeWatch(const string &location)
{
	map<string, int>::iterator locationIter = m_locations.find(location);
	if (locationIter != m_locations.end())
	{
		inotify_rm_watch(m_monitorFd, locationIter->second);
		m_noWatchesLeft = false;

		map<int, string>::iterator watchIter = m_watches.find(locationIter->second);
		if (watchIter != m_watches.end())
		{
			m_watches.erase(watchIter);
		}
		m_locations.erase(locationIter);

		return true;
	}
	else
	{
		cerr << location << " is not being monitored" << endl;
	}

	return false;
}

/// Starts monitoring a location.
bool INotifyMonitor::addLocation(const string &location, bool isDirectory)
{
	uint32_t eventsMask = IN_CLOSE_WRITE|IN_MOVE|IN_CREATE|IN_DELETE|IN_UNMOUNT|IN_MOVE_SELF|IN_DELETE_SELF;
	bool addedLocation = false;

	if ((location.empty() == true) ||
		(location == "/") ||
		(m_monitorFd < 0) ||
		(m_noWatchesLeft == true))
	{
		return false;
	}

	if (pthread_mutex_lock(&m_mutex) != 0)
	{
		return false;
	}

	map<string, int>::iterator locationIter = m_locations.find(location);
	if (locationIter != m_locations.end())
	{
		// This is already being monitored
		addedLocation = true;
	}
	else
	{
		// FIXME: check the maximum number of watches hasn't been reached (MAX_FILE_WATCHES ?)
		int watchNum = inotify_add_watch(m_monitorFd, location.c_str(), eventsMask);
		if (watchNum >= 0)
		{
			// Generate an event to signal the file exists and is being monitored
			if (isDirectory == false)
			{
				MonitorEvent monEvent;
				monEvent.m_location = location;
				monEvent.m_isWatch = true;
				monEvent.m_type = MonitorEvent::EXISTS;
				monEvent.m_isDirectory = false;
				m_internalEvents.push(monEvent);
			}

			m_watches.insert(pair<int, string>(watchNum, location));
			m_locations.insert(pair<string, int>(location, watchNum));
#ifdef DEBUG
			cout << "INotifyMonitor::addLocation: added watch "
				<< watchNum << " for " << location << endl;
#endif
			addedLocation = true;
		}
		else
		{
			if (errno == ENOSPC)
			{
				m_noWatchesLeft = true;
			}
			cerr << "Couldn't monitor " << location << endl;
		}
	}
	pthread_mutex_unlock(&m_mutex);

	return addedLocation;
}

/// Stops monitoring a location.
bool INotifyMonitor::removeLocation(const string &location)
{
	bool removedLocation = false;

	if ((location.empty() == true) ||
		(m_monitorFd < 0))
	{
		return false;
	}

	if (pthread_mutex_lock(&m_mutex) != 0)
	{
		return false;
	}

	removedLocation = removeWatch(location);
	pthread_mutex_unlock(&m_mutex);

	return removedLocation;
}

/// Retrieves pending events.
bool INotifyMonitor::retrievePendingEvents(queue<MonitorEvent> &events)
{
	set<string> removedLocations;
	char buffer[1024];
	unsigned int queueLen = 0;
	size_t offset = 0;

	if (m_monitorFd < 0)
	{
		return false;
	}

	if (pthread_mutex_lock(&m_mutex) != 0)
	{
		return false;
	}

	// Copy internal events
	while (m_internalEvents.empty() == false)
	{
		MonitorEvent &internalEvent = m_internalEvents.front();
		events.push(internalEvent);

		// Next
		m_internalEvents.pop();
	}

	if (ioctl(m_monitorFd, FIONREAD, &queueLen) == 0)
	{
#ifdef DEBUG
		cout << "INotifyMonitor::retrievePendingEvents: "
			<< queueLen << " bytes to read" << endl;
#endif
	}
	if (queueLen == 0)
	{
		// Nothing to read
		pthread_mutex_unlock(&m_mutex);
		return true;
	}

	int bytesRead = read(m_monitorFd, buffer, 1024);
	while ((bytesRead > 0) &&
		(bytesRead - offset > 0))
	{
		struct inotify_event *pEvent = (struct inotify_event *)&buffer[offset];
		size_t eventSize = sizeof(struct inotify_event) + pEvent->len;

#ifdef DEBUG
		cout << "INotifyMonitor::retrievePendingEvents: read "
			<< bytesRead << " bytes at offset " << offset << endl;
#endif

		// What location is this event for ?
		map<int, string>::iterator watchIter = m_watches.find(pEvent->wd);
		if (watchIter == m_watches.end())
		{
#ifdef DEBUG
			cout << "INotifyMonitor::retrievePendingEvents: unknown watch "
				<< pEvent->wd << endl;
#endif
			offset += eventSize;
			continue;
		}

		MonitorEvent monEvent;

		monEvent.m_isWatch = true;
		if (pEvent->mask & IN_ISDIR)
		{
			monEvent.m_isDirectory = true;
		}

		monEvent.m_location = watchIter->second;
		// A name is provided if the target is below a location we match
		if (pEvent->len >= 1)
		{
			monEvent.m_location += "/";
			monEvent.m_location += pEvent->name;
			monEvent.m_isWatch = false;
		}

		// What type of event ?
		if (pEvent->mask & IN_CREATE)
		{
#ifdef DEBUG
			cout << "INotifyMonitor::retrievePendingEvents: created "
				<< monEvent.m_location << endl;
#endif
			monEvent.m_type = MonitorEvent::CREATED;
		}
		else if (pEvent->mask & IN_CLOSE_WRITE)
		{
#ifdef DEBUG
			cout << "INotifyMonitor::retrievePendingEvents: written and closed "
				<< monEvent.m_location << endl;
#endif
			monEvent.m_type = MonitorEvent::WRITE_CLOSED;
		}
		else if (pEvent->mask & IN_MOVED_FROM)
		{
#ifdef DEBUG
			cout << "INotifyMonitor::retrievePendingEvents: moved from on "
				<< monEvent.m_location << " " << pEvent->cookie << endl;
#endif
			// Store this until we receive a IN_MOVED_TO event
			m_movedFrom.insert(pair<uint32_t, MonitorEvent>(pEvent->cookie, monEvent));
		}
		else if (pEvent->mask & IN_MOVED_TO)
		{
#ifdef DEBUG
			cout << "INotifyMonitor::retrievePendingEvents: moved to on "
				<< monEvent.m_location << " " << pEvent->cookie << endl;
#endif
			// What was the previous location ?
			map<uint32_t, MonitorEvent>::iterator movedIter = m_movedFrom.find(pEvent->cookie);
			if (movedIter != m_movedFrom.end())
			{
				monEvent.m_previousLocation = movedIter->second.m_location;
				monEvent.m_type = MonitorEvent::MOVED;
#ifdef DEBUG
				cout << "INotifyMonitor::retrievePendingEvents: moved from "
					<< monEvent.m_previousLocation << endl;
#endif
				m_movedFrom.erase(movedIter);

				// Has a watch moved ?
				if ((monEvent.m_isWatch == true) &&
					(monEvent.m_previousLocation == watchIter->second))
				{
					// Update the location for this watch
					map<string, int>::iterator locationIter = m_locations.find(watchIter->second);
					if (locationIter != m_locations.end())
					{
						m_locations.erase(locationIter);
						m_locations[monEvent.m_location] = pEvent->wd;
					}
					watchIter->second = monEvent.m_location;
				}
			}
			else
			{
				// The previous location is unknown because it's from somewhere not being monitored
				monEvent.m_type = MonitorEvent::CREATED;
#ifdef DEBUG
				cout << "INotifyMonitor::retrievePendingEvents: don't know where file was moved from" << endl;
#endif
			}
		}
		else if (pEvent->mask & IN_MOVE_SELF)
		{
			map<uint32_t, MonitorEvent>::iterator movedIter = m_movedFrom.end();

#ifdef DEBUG
			cout << "INotifyMonitor::retrievePendingEvents: moved self on "
				<< monEvent.m_location << " " << pEvent->cookie << endl;
#endif
			// It was moved somewhere not being monitored
			if (pEvent->cookie == 0)
			{
				for (movedIter = m_movedFrom.begin(); movedIter != m_movedFrom.end(); ++movedIter)
				{
					if (movedIter->second.m_location == monEvent.m_location)
					{
						// For some reason, IN_ISDIR is not set when the cookie is 0
						if (movedIter->second.m_isDirectory == true)
						{
							monEvent.m_isDirectory = true;
						}
						break;
					}
				}
			}
			else
			{
				movedIter = m_movedFrom.find(pEvent->cookie);
			}

			if (movedIter != m_movedFrom.end())
			{
				monEvent.m_type = MonitorEvent::DELETED;

				m_movedFrom.erase(movedIter);
			}
		}
		else if (pEvent->mask & IN_DELETE)
		{
#ifdef DEBUG
			cout << "INotifyMonitor::retrievePendingEvents: deleted "
				<< monEvent.m_location << endl;
#endif
			monEvent.m_type = MonitorEvent::DELETED;
		}
		else if (pEvent->mask & IN_DELETE_SELF)
		{
#ifdef DEBUG
			cout << "INotifyMonitor::retrievePendingEvents: deleted self on "
				<< monEvent.m_location << endl;
#endif
			if (monEvent.m_isWatch == true)
			{
				removedLocations.insert(monEvent.m_location);
			}
		}
		else if (pEvent->mask & IN_UNMOUNT)
		{
#ifdef DEBUG
			cout << "INotifyMonitor::retrievePendingEvents: unmounted on "
				<< monEvent.m_location << endl;
#endif
			if (monEvent.m_isWatch == true)
			{
				// Watches are removed silently if the backing filesystem is unmounted
				removedLocations.insert(monEvent.m_location);
			}
		}
		else
		{
#ifdef DEBUG
			cout << "INotifyMonitor::retrievePendingEvents: ignoring event "
				<< pEvent->mask << " on " << monEvent.m_location << endl;
#endif
		}

		// Return event ?
		if (monEvent.m_type != MonitorEvent::UNKNOWN)
		{
			events.push(monEvent);
		}

		// Any IN_MOVED_FROM event for which we didn't get a IN_MOVED_TO ?
		time_t now = time(NULL);
		map<uint32_t, MonitorEvent>::iterator movedIter = m_movedFrom.begin();
		while (movedIter != m_movedFrom.end())
		{
			// The file was probably moved to an unmonitored location on the same filesystem
			if (movedIter->second.m_time + 60 < now)
			{
				// It's as good as if it was deleted
				movedIter->second.m_type = MonitorEvent::DELETED;
				events.push(movedIter->second);
#ifdef DEBUG
				cout << "INotifyMonitor::retrievePendingEvents: don't know where "
					<< movedIter->second.m_location << " was moved to" << endl;
#endif

				map<uint32_t, MonitorEvent>::iterator nextMovedIter = movedIter;
				++nextMovedIter;
				m_movedFrom.erase(movedIter);
				movedIter = nextMovedIter;
			}
			else
			{
				++movedIter;
			}
		}

		offset += eventSize;
	}
	// Any location to remove ?
	for (set<string>::const_iterator removalIter = removedLocations.begin();
		removalIter != removedLocations.end(); ++removalIter)
	{
		removeWatch(*removalIter);
	}
	pthread_mutex_unlock(&m_mutex);

	return true;
}


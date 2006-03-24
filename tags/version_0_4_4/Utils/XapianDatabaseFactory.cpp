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

#include "XapianDatabaseFactory.h"

using std::cout;
using std::endl;
using std::string;
using std::map;
using std::pair;

pthread_mutex_t XapianDatabaseFactory::m_mutex = PTHREAD_MUTEX_INITIALIZER;
map<string, XapianDatabase *> XapianDatabaseFactory::m_databases;

XapianDatabaseFactory::XapianDatabaseFactory()
{
}

XapianDatabaseFactory::~XapianDatabaseFactory()
{
}

/// Returns a XapianDatabase pointer; NULL if unavailable.
XapianDatabase *XapianDatabaseFactory::getDatabase(const string &location, bool readOnly)
{
	XapianDatabase *pDb = NULL;

	if (location.empty() == true)
	{
		return NULL;
	}

	// Lock the map
	if (pthread_mutex_lock(&m_mutex) != 0)
	{
		return NULL;
	}

	map<string, XapianDatabase *>::iterator dbIter = m_databases.find(location);
	if (dbIter != m_databases.end())
	{
#ifdef DEBUG
		cout << "XapianDatabaseFactory::getDatabase: found " << location << endl;
#endif
		pDb = dbIter->second;
	}
	else
	{
#ifdef DEBUG
		cout << "XapianDatabaseFactory::getDatabase: opening " << location
			<< " in mode " << readOnly << endl;
#endif
		// Create a new instance
		pDb = new XapianDatabase(location, readOnly);
		// Insert it into the map
		pair<map<string, XapianDatabase *>::iterator, bool> insertPair = m_databases.insert(pair<string, XapianDatabase *>(location, pDb));
		// Was it inserted ?
		if (insertPair.second == false)
		{
			// No, it wasn't : delete the object
			delete pDb;
			pDb = NULL;
		}
	}

	// Unlock the map
	pthread_mutex_unlock(&m_mutex);

	return pDb;
}

/// Closes all databases.
void XapianDatabaseFactory::closeAll(void)
{
	if (m_databases.empty() == true)
	{
		return;
	}

	// Lock the map
	if (pthread_mutex_lock(&m_mutex) != 0)
	{
		return;
	}

	std::map<std::string, XapianDatabase *>::iterator dbIter = m_databases.begin();
	while (dbIter != m_databases.end())
	{
		XapianDatabase *pDb = dbIter->second;
		dbIter->second = NULL;
#ifdef DEBUG
		cout << "XapianDatabaseFactory::closeAll: closing " << dbIter->first << endl;
#endif
		m_databases.erase(dbIter);
		delete pDb;

		dbIter = m_databases.begin();
	}

	// Unlock the map
	pthread_mutex_unlock(&m_mutex);
}
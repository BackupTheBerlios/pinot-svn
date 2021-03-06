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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

#include "Url.h"
#include "ViewHistory.h"

ViewHistory::ViewHistory(const string &database) :
	SQLiteBase(database)
{
}

ViewHistory::~ViewHistory()
{
}

/// Creates the ViewHistory table in the database.
bool ViewHistory::create(const string &database)
{
	bool success = true;

	// The specified path must be a file
	if (SQLiteBase::check(database) == false)
	{
		return false;
	}

	SQLiteBase db(database);

	// Does ViewHistory exist ?
	if (db.executeSimpleStatement("SELECT * FROM ViewHistory LIMIT 1;") == false)
	{
#ifdef DEBUG
		cout << "ViewHistory::create: ViewHistory doesn't exist" << endl;
#endif
		// Create the table
		if (db.executeSimpleStatement("CREATE TABLE ViewHistory (Url VARCHAR(255) PRIMARY KEY, Status INT);") == false)
		{
			success = false;
		}
	}

	return success;
}

/// Inserts an URL.
bool ViewHistory::insertItem(const string &url)
{
	bool success = false;

	SQLiteResults *results = executeStatement("INSERT INTO ViewHistory \
		VALUES('%q', '1');", Url::escapeUrl(url).c_str());
	if (results != NULL)
	{
		success = true;
		delete results;
	}

	return success;
}

/// Checks if an URL is in the history.
bool ViewHistory::hasItem(const string &url) const
{
	bool success = false;

	SQLiteResults *results = executeStatement("SELECT Url FROM ViewHistory WHERE Url='%q';",
		Url::escapeUrl(url).c_str());
	if (results != NULL)
	{
		SQLiteRow *row = results->nextRow();
		if (row != NULL)
		{
			// If this returns anything, it's the URL we are looking for
#ifdef DEBUG
			cout << "ViewHistory::hasItem: URL " << row->getColumn(0) << endl;
#endif
			success = true;

			delete row;
		}

		delete results;
	}

	return success;
}

/// Deletes an URL.
bool ViewHistory::deleteItem(const string &url)
{
	bool success = false;

	SQLiteResults *results = executeStatement("DELETE FROM ViewHistory \
		WHERE Url='%q';", Url::escapeUrl(url).c_str());
	if (results != NULL)
	{
		success = true;
		delete results;
	}

	return success;
}

/*
 *  Copyright 2005-2009 Fabrice Colin
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
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <set>
#include <iostream>

#include "Url.h"
#include "StringManip.h"
#include "TimeConverter.h"
#include "ActionQueue.h"

using std::string;
using std::set;
using std::cout;
using std::endl;

ActionQueue::ActionQueue(const string &database, const string queueId) :
	SQLiteBase(database),
	m_queueId(queueId)
{
}

ActionQueue::~ActionQueue()
{
}

string ActionQueue::typeToText(ActionType type)
{
	string text;

	switch (type)
	{
		case INDEX:
			text = "INDEX";
			break;
		case UNINDEX:
			text = "UNINDEX";
			break;
		default:
			break;
	}

	return text;
}

ActionQueue::ActionType ActionQueue::textToType(const string &text)
{
	ActionType type = INDEX;

	if (text == "UNINDEX")
	{
		type = UNINDEX;
	}

	return type;
}

/// Creates the ActionQueue table in the database.
bool ActionQueue::create(const string &database)
{
	bool success = true;

	// The specified path must be a file
	if (SQLiteBase::check(database) == false)
	{
		return false;
	}

	SQLiteBase db(database);

	// Does ActionQueue exist ?
	if (db.executeSimpleStatement("SELECT * FROM ActionQueue LIMIT 1;") == false)
	{
#ifdef DEBUG
		cout << "ActionQueue::create: ActionQueue doesn't exist" << endl;
#endif
		// Create the table
		if (db.executeSimpleStatement("CREATE TABLE ActionQueue (QueueId VARCHAR(255), \
			Url VARCHAR(255), Type VARCHAR(255), Date INTEGER, Info TEXT, \
			PRIMARY KEY(QueueId, Url));") == false)
		{
			success = false;
		}
	}

	return success;
}

/// Pushes an item.
bool ActionQueue::pushItem(ActionType type, const DocumentInfo &docInfo)
{
	string url(docInfo.getLocation());
	string info(docInfo.serialize());
	bool update = false, success = false;

	// Is there already an item for this URL ?
	SQLResults *results = executeStatement("SELECT Url FROM ActionQueue \
		WHERE QueueId='%q' AND Url='%q';",
		m_queueId.c_str(), Url::escapeUrl(url).c_str());
	if (results != NULL)
	{
		SQLRow *row = results->nextRow();
		if (row != NULL)
		{
#ifdef DEBUG
			cout << "ActionQueue::pushItem: item "
				<< Url::unescapeUrl(row->getColumn(0)) << " exists" << endl;
#endif
			update = true;

			delete row;
		}

		delete results;
	}

	if (update == false)
	{
		results = executeStatement("INSERT INTO ActionQueue \
			VALUES('%q', '%q', '%q', '%d', '%q');",
			m_queueId.c_str(), Url::escapeUrl(url).c_str(),
			typeToText(type).c_str(), time(NULL), info.c_str());
	}
	else
	{
		results = executeStatement("UPDATE ActionQueue \
			SET Type='%q', Date='%d', Info='%q' WHERE \
			QueueId='%q' AND Url='%q';",
			typeToText(type).c_str(), time(NULL), info.c_str(),
			m_queueId.c_str(), Url::escapeUrl(url).c_str());
	}
	if (results != NULL)
	{
#ifdef DEBUG
		cout << "ActionQueue::pushItem: queue " << m_queueId
			<< ": " << type << " on " << url << ", " << update << endl;
#endif
		success = true;

		delete results;
	}

	return success;
}

/// Pops and deletes the oldest item.
bool ActionQueue::popItem(ActionType &type, DocumentInfo &docInfo)
{
	string url;
	bool success = false;

	if (getOldestItem(type, docInfo) == false)
	{
		return false;
	}
	url = docInfo.getLocation();
#ifdef DEBUG
	cout << "ActionQueue::popItem: queue " << m_queueId
		<< ": " << type << " on " << url << endl;
#endif

	// Delete from ActionQueue
	SQLResults *results = executeStatement("DELETE FROM ActionQueue \
		WHERE QueueId='%q' AND Url='%q';",
		m_queueId.c_str(), Url::escapeUrl(url).c_str());
	if (results != NULL)
	{
		success = true;
		delete results;
	}

	return success;
}

bool ActionQueue::getOldestItem(ActionType &type, DocumentInfo &docInfo)
{
	bool success = false;

	SQLResults *results = executeStatement("SELECT Type, Info FROM ActionQueue \
		WHERE QueueId='%q' ORDER BY Date DESC LIMIT 1;",
		m_queueId.c_str());
	if (results != NULL)
	{
		SQLRow *row = results->nextRow();
		if (row != NULL)
		{
			type = textToType(row->getColumn(0));
			success = true;

			// Deserialize DocumentInfo
			docInfo.deserialize(row->getColumn(1));

			delete row;
		}

		delete results;
	}

	return success;
}

/// Returns the number of items of a particular type.
unsigned int ActionQueue::getItemsCount(ActionType type)
{
	unsigned int count = 0;

	SQLResults *results = executeStatement("SELECT COUNT(*) FROM ActionQueue \
		WHERE Type='%q';",
		typeToText(type).c_str());
	if (results != NULL)
	{
		SQLRow *row = results->nextRow();
		if (row != NULL)
		{
			count = atoi(row->getColumn(0).c_str());

			delete row;
		}

		delete results;
	}

	return count;
}

/// Expires items older than the given date.
bool ActionQueue::expireItems(time_t expiryDate)
{
	bool success = false;

	SQLResults *results = executeStatement("DELETE FROM ActionQueue \
		WHERE QueueId='%q' AND Date<'%d';",
		m_queueId.c_str(), expiryDate);
	if (results != NULL)
	{
		success = true;
		delete results;
	}

	return success;
}

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

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sstream>

#include "Url.h"
#include "CrawlHistory.h"

using std::clog;
using std::endl;
using std::string;
using std::set;
using std::map;
using std::vector;
using std::stringstream;

CrawlHistory::CrawlHistory(const string &database) :
	SQLiteBase(database, false, false)
{
	prepareStatement("has-item",
		"SELECT Status, Date FROM CrawlHistory WHERE Url=?;");
	prepareStatement("get-items",
		"SELECT Url FROM CrawlHistory WHERE Status=?;");
	prepareStatement("get-source-items1",
		"SELECT Url FROM CrawlHistory WHERE SourceId=? AND Status=? AND Date>? LIMIT ? OFFSET ?;");
	prepareStatement("get-source-items2",
		"SELECT Url FROM CrawlHistory WHERE SourceId=? AND Status=? LIMIT ? OFFSET ?;");
}

CrawlHistory::~CrawlHistory()
{
}

string CrawlHistory::statusToText(CrawlStatus status)
{
	string text;

	switch (status)
	{
		case UNKNOWN:
			text = "UNKNOWN";
			break;
		case TO_CRAWL:
			text = "TO_CRAWL";
			break;
		case CRAWLING:
			text = "CRAWLING";
			break;
		case CRAWLED:
			text = "CRAWLED";
			break;
		case CRAWL_ERROR:
			text = "ERROR";
			break;
		case CRAWL_LINK:
			text = "LINK";
			break;
		default:
			break;
	}

	return text;
}

CrawlHistory::CrawlStatus CrawlHistory::textToStatus(const string &text)
{
	CrawlStatus status = UNKNOWN;

	if (text == "TO_CRAWL")
	{
		status = TO_CRAWL;
	}
	else if (text == "CRAWLING")
	{
		status = CRAWLING;
	}
	else if (text == "CRAWLED")
	{
		status = CRAWLED;
	}
	else if (text == "ERROR")
	{
		status = CRAWL_ERROR;
	}
	else if (text == "LINK")
	{
		status = CRAWL_LINK;
	}

	return status;
}

/// Creates the CrawlHistory table in the database.
bool CrawlHistory::create(const string &database)
{
	bool createHistoryTable = false;

	// The specified path must be a file
	if (SQLiteBase::check(database) == false)
	{
		return false;
	}

	SQLiteBase db(database);

	// Does CrawlSources exist ?
	if (db.executeSimpleStatement("SELECT * FROM CrawlSources LIMIT 1;") == false)
	{
		if (db.executeSimpleStatement("CREATE TABLE CrawlSources (SourceID INTEGER \
			PRIMARY KEY, Url VARCHAR(255));") == false)
		{
			return false;
		}
	}

	// Does CrawlHistory exist ?
	if (db.executeSimpleStatement("SELECT * FROM CrawlHistory LIMIT 1;") == false)
	{
		createHistoryTable = true;
	}
	else
	{
		// Previous versions didn't include a ErrorNum field, so check for it
		if (db.executeSimpleStatement("SELECT ErrorNum FROM CrawlHistory LIMIT 1;") == false)
		{
#ifdef DEBUG
			clog << "CrawlHistory::create: CrawlHistory needs updating" << endl;
#endif
			// Ideally, we would use ALTER TABLE but it's not supported by SQLite
			if (db.executeSimpleStatement("DROP TABLE CrawlHistory; VACUUM;") == true)
			{
				createHistoryTable = true;
			}
		}
	}

	if (createHistoryTable == true)
	{
		if (db.executeSimpleStatement("CREATE TABLE CrawlHistory (Url VARCHAR(255) PRIMARY KEY, \
			Status VARCHAR(255), SourceID INTEGER, Date INTEGER, ErrorNum INTEGER);") == false)
		{
			return false;
		}
	}

	return true;
}

/// Inserts a source.
unsigned int CrawlHistory::insertSource(const string &url)
{
	unsigned int sourceId = 0;

	SQLResults *results = executeStatement("SELECT MAX(SourceID) FROM CrawlSources;");
	if (results != NULL)
	{
		SQLRow *row = results->nextRow();
		if (row != NULL)
		{
			sourceId = atoi(row->getColumn(0).c_str());
			++sourceId;

			delete row;
		}

		delete results;
	}

	results = executeStatement("INSERT INTO CrawlSources \
		VALUES('%u', '%q');",
		sourceId, Url::escapeUrl(url).c_str());
	if (results != NULL)
	{
		delete results;
	}

	return sourceId;
}

/// Checks if the source exists.
bool CrawlHistory::hasSource(const string &url, unsigned int &sourceId)
{
	bool success = false;

	SQLResults *results = executeStatement("SELECT SourceID FROM CrawlSources \
		WHERE Url='%q';", Url::escapeUrl(url).c_str());
	if (results != NULL)
	{
		SQLRow *row = results->nextRow();
		if (row != NULL)
		{
			sourceId = atoi(row->getColumn(0).c_str());
			success = true;

			delete row;
		}

		delete results;
	}

	return success;
}

/// Returns sources.
unsigned int CrawlHistory::getSources(map<unsigned int, string> &sources)
{
	unsigned int count = 0;

	SQLResults *results = executeStatement("SELECT SourceID, Url FROM CrawlSources;");
	if (results != NULL)
	{
		while (results->hasMoreRows() == true)
		{
			SQLRow *row = results->nextRow();
			if (row == NULL)
			{
				break;
			}

			sources[(unsigned int)atoi(row->getColumn(0).c_str())] = Url::unescapeUrl(row->getColumn(1));
			++count;

			delete row;
		}

		delete results;
	}

	return count;
}

/// Deletes a source.
bool CrawlHistory::deleteSource(unsigned int sourceId)
{
	bool success = false;

	SQLResults *results = executeStatement("DELETE FROM CrawlSources \
		WHERE SourceID='%u';", sourceId);
	if (results != NULL)
	{
		success = true;
		delete results;
	}

	return success;
}

/// Inserts an URL.
bool CrawlHistory::insertItem(const string &url, CrawlStatus status, unsigned int sourceId,
	time_t date, int errNum)
{
	bool success = false;

	SQLResults *results = executeStatement("INSERT INTO CrawlHistory \
		VALUES('%q', '%q', '%u', '%d', '%d');",
		Url::escapeUrl(url).c_str(), statusToText(status).c_str(), sourceId,
		(date == 0 ? time(NULL) : date), errNum);
	if (results != NULL)
	{
		success = true;
		delete results;
	}

	return success;
}

/// Checks if an URL is in the history.
bool CrawlHistory::hasItem(const string &url, CrawlStatus &status, time_t &date)
{
	vector<string> values;
	bool success = false;

	values.push_back(Url::escapeUrl(url));

	SQLResults *results = executePreparedStatement("has-item", values);
	if (results != NULL)
	{
		SQLRow *row = results->nextRow();
		if (row != NULL)
		{
			status = textToStatus(row->getColumn(0));
			date = (time_t)atoi(row->getColumn(1).c_str());
			success = true;

			delete row;
		}

		delete results;
	}

	return success;
}

/// Updates an URL.
bool CrawlHistory::updateItem(const string &url, CrawlStatus status, time_t date, int errNum)
{
	bool success = false;

	SQLResults *results = executeStatement("UPDATE CrawlHistory \
		SET Status='%q', Date='%d', ErrorNum='%d' WHERE Url='%q';",
		statusToText(status).c_str(), (date == 0 ? time(NULL) : date), errNum,
		Url::escapeUrl(url).c_str());
	if (results != NULL)
	{
		success = true;
		delete results;
	}

	return success;
}

/// Updates URLs.
bool CrawlHistory::updateItems(const map<string, CrawlItem> &items)
{
	bool success = false;

	if (beginTransaction() == false)
	{
		return false;
	}

	for (map<string, CrawlItem>::const_iterator updateIter = items.begin();
		updateIter != items.end(); ++updateIter)
	{
		SQLResults *results = executeStatement("UPDATE CrawlHistory \
			SET Status='%q', Date='%d', ErrorNum='%d' WHERE Url='%q';",
			statusToText(updateIter->second.m_itemStatus).c_str(),
			(updateIter->second.m_itemDate == 0 ? time(NULL) : updateIter->second.m_itemDate),
			updateIter->second.m_errNum,
			Url::escapeUrl(updateIter->first).c_str());
		if (results != NULL)
		{
			success = true;
			delete results;
		}
	}

	if (endTransaction() == false)
	{
		return false;
	}

	return success;
}

/// Updates the status of items en masse.
bool CrawlHistory::updateItemsStatus(CrawlStatus oldStatus, CrawlStatus newStatus,
	unsigned int sourceId, bool allSources)
{
	SQLResults *results = NULL;
	bool success = false;

	if (beginTransaction() == false)
	{
		return false;
	}

	if (allSources == false)
	{
		results = executeStatement("UPDATE CrawlHistory \
			SET Status='%q' WHERE SourceId='%u' AND Status='%q';",
			statusToText(newStatus).c_str(), sourceId, statusToText(oldStatus).c_str());
	}
	else
	{
		// Ignore the source
		results = executeStatement("UPDATE CrawlHistory \
			SET Status='%q' WHERE Status='%q';",
			statusToText(newStatus).c_str(), statusToText(oldStatus).c_str());
	}

	if (results != NULL)
	{
		success = true;
		delete results;
	}

	if (endTransaction() == false)
	{
		return false;
	}

	return success;
}

/// Gets the error number and date for a URL.
int CrawlHistory::getErrorDetails(const string &url, time_t &date)
{
	int errNum = 0;

	SQLResults *results = executeStatement("SELECT ErrorNum, Date FROM CrawlHistory WHERE Url='%q';",
		Url::escapeUrl(url).c_str());
	if (results != NULL)
	{
		SQLRow *row = results->nextRow();
		if (row != NULL)
		{
			errNum = atoi(row->getColumn(0).c_str());
			date = (time_t)atoi(row->getColumn(1).c_str());

			delete row;
		}

		delete results;
	}

	return errNum;
}

/// Returns items.
unsigned int CrawlHistory::getItems(CrawlStatus status, set<string> &urls)
{
	vector<string> values;
	unsigned int count = 0;

	values.push_back(statusToText(status));

	SQLResults *results = executePreparedStatement("get-items", values);
	if (results != NULL)
	{
		while (results->hasMoreRows() == true)
		{
			SQLRow *row = results->nextRow();
			if (row == NULL)
			{
				break;
			}

			urls.insert(Url::unescapeUrl(row->getColumn(0)));
			++count;

			delete row;
		}

		delete results;
	}

	return count;
}

/// Returns items that belong to a source.
unsigned int CrawlHistory::getSourceItems(unsigned int sourceId, CrawlStatus status,
	set<string> &urls, unsigned int min, unsigned int max,
	time_t minDate)
{
	vector<string> values;
	stringstream numStr;
	SQLResults *results = NULL;
	unsigned int count = 0;

	numStr << sourceId;
	values.push_back(numStr.str());
	values.push_back(statusToText(status));
	if (minDate > 0)
	{
		numStr.clear();
		numStr << minDate;
		values.push_back(numStr.str());
		numStr.clear();
		numStr << max - min;
		values.push_back(numStr.str());
		numStr.clear();
		numStr << min;
		values.push_back(numStr.str());

		results = executePreparedStatement("get-source-items1", values);
	}
	else
	{
		numStr.clear();
		numStr << max - min;
		values.push_back(numStr.str());
		numStr.clear();
		numStr << min;
		values.push_back(numStr.str());

		// Ignore the date
		results = executePreparedStatement("get-source-items2", values);
	}

	if (results != NULL)
	{
		while (results->hasMoreRows() == true)
		{
			SQLRow *row = results->nextRow();
			if (row == NULL)
			{
				break;
			}

			urls.insert(Url::unescapeUrl(row->getColumn(0)));
			++count;

			delete row;
		}

		delete results;
	}

	return count;
}

/// Returns the number of URLs.
unsigned int CrawlHistory::getItemsCount(CrawlStatus status)
{
	unsigned int count = 0;

	SQLResults *results = executeStatement("SELECT COUNT(*) FROM CrawlHistory WHERE Status='%q';",
		statusToText(status).c_str());
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

/// Deletes an URL.
bool CrawlHistory::deleteItem(const string &url)
{
	bool success = false;

	SQLResults *results = executeStatement("DELETE FROM CrawlHistory \
		WHERE Url='%q';", Url::escapeUrl(url).c_str());
	if (results != NULL)
	{
		success = true;
		delete results;
	}

	return success;
}

/// Deletes all items under a given URL.
bool CrawlHistory::deleteItems(const string &url)
{
	bool success = false;

	if (beginTransaction() == false)
	{
		return false;
	}

	SQLResults *results = executeStatement("DELETE FROM CrawlHistory \
		WHERE Url LIKE '%q%%';", Url::escapeUrl(url).c_str());
	if (results != NULL)
	{
		success = true;
		delete results;
	}

	if (endTransaction() == false)
	{
		return false;
	}

	return success;
}

/// Deletes URLs belonging to a source.
bool CrawlHistory::deleteItems(unsigned int sourceId, CrawlStatus status)
{
	SQLResults *results = NULL;
	bool success = false;

	if (beginTransaction() == false)
	{
		return false;
	}

	if (status == UNKNOWN)
	{
		results = executeStatement("DELETE FROM CrawlHistory \
			WHERE SourceID='%u';", sourceId);
	}
	else
	{
		results = executeStatement("DELETE FROM CrawlHistory \
			WHERE SourceID='%u' AND Status='%q';",
			sourceId, statusToText(status).c_str());
	}

	if (results != NULL)
	{
		success = true;
		delete results;
	}

	if (endTransaction() == false)
	{
		return false;
	}

	return success;
}

/// Expires items older than the given date.
bool CrawlHistory::expireItems(time_t expiryDate)
{
	bool success = false;

	if (beginTransaction() == false)
	{
		return false;
	}

	SQLResults *results = executeStatement("DELETE FROM CrawlHistory \
		WHERE Date<'%d';", expiryDate);
	if (results != NULL)
	{
		success = true;
		delete results;
	}

	if (endTransaction() == false)
	{
		return false;
	}

	return success;
}


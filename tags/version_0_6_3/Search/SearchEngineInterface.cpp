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

#include <iostream>

#include "Document.h"
#include "StringManip.h"
#include "Url.h"
#include "SearchEngineInterface.h"

using std::cout;
using std::endl;

SearchEngineInterface::SearchEngineInterface() :
	m_callsCount(0),
	m_startTime(time(NULL)),
	m_maxResultsCount(10)
{
}

SearchEngineInterface::~SearchEngineInterface()
{
}

/// Sets the search engine's key, if applicable.
void SearchEngineInterface::setKey(const string &key)
{
	m_key = key;
}

/// Sets the number of calls issued since start time.
void SearchEngineInterface::setCallsCount(unsigned int count, time_t startTime)
{
	m_callsCount = count;
	m_startTime = startTime;
}

/// Sets the maximum number of results to return.
void SearchEngineInterface::setMaxResultsCount(unsigned int count)
{
	m_maxResultsCount = count;
}

/// Sets whether the query should be expanded.
bool SearchEngineInterface::setQueryExpansion(set<unsigned int> &relevantDocuments)
{
	// Not all engines support this
	return false;
}

/// Returns the results for the previous query.
const vector<Result> &SearchEngineInterface::getResults(void) const
{
	return m_resultsList;
}

/// Returns the charset for the previous query's results.
string SearchEngineInterface::getResultsCharset(void) const
{
	return m_charset;
}

/// Returns expand terms from the previous query.
const set<string> &SearchEngineInterface::getExpandTerms(void) const
{
	return m_expandTerms;
}

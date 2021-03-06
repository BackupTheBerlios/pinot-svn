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

#include <ctype.h>
#include <set>
#include <iostream>
#include <algorithm>

#include "Tokenizer.h"
#include "XapianEngine.h"
#include "QueryProperties.h"

QueryProperties::QueryProperties() :
	m_resultsCount(10),
	m_indexResults(false)
{
}

QueryProperties::QueryProperties(const string &name, const string &freeQuery) :
	m_name(name),
	m_freeQuery(freeQuery),
	m_resultsCount(10),
	m_indexResults(false)
{
	removeFilters();
}

QueryProperties::QueryProperties(const QueryProperties &other) :
	m_name(other.m_name),
	m_freeQuery(other.m_freeQuery),
	m_freeQueryWithoutFilters(other.m_freeQueryWithoutFilters),
	m_language(other.m_language),
	m_hostFilter(other.m_hostFilter),
	m_fileFilter(other.m_fileFilter),
	m_resultsCount(other.m_resultsCount),
	m_indexResults(other.m_indexResults),
	m_labelName(other.m_labelName)
{
}

QueryProperties::~QueryProperties()
{
}

QueryProperties &QueryProperties::operator=(const QueryProperties &other)
{
	if (this != &other)
	{
		m_name = other.m_name;
		m_freeQuery = other.m_freeQuery;
		m_freeQueryWithoutFilters = other.m_freeQueryWithoutFilters;
		m_language = other.m_language;
		m_hostFilter = other.m_hostFilter;
		m_fileFilter = other.m_fileFilter;
		m_resultsCount = other.m_resultsCount;
		m_indexResults = other.m_indexResults;
		m_labelName = other.m_labelName;
	}

	return *this;
}

bool QueryProperties::operator==(const QueryProperties &other) const
{
	if (m_name == other.m_name)
	{
		return true;
	}

	return false;
}

bool QueryProperties::operator<(const QueryProperties &other) const
{
	if (m_name < other.m_name)
	{
		return true;
	}

	return false;
}

void QueryProperties::removeFilters(void)
{
	m_freeQueryWithoutFilters.clear();
	m_language.clear();
	m_hostFilter.clear();
	m_fileFilter.clear();

	if (m_freeQuery.empty() == true)
	{
		return;
	}

	set<string> terms;
	if (XapianEngine::validateQuery(*this, true, terms) == true)
	{
		for (set<string>::const_iterator termIter = terms.begin();
			termIter != terms.end(); ++termIter)
		{
			string term(*termIter);

			if (term.empty() == true)
			{
				continue;
			}

			// is this a prefixed term ?
			// The query shouldn't have the same filter twice
			if (isupper((int)((*termIter)[0])) == 0)
			{
				// FIXME: operators have been lost !
				if (m_freeQueryWithoutFilters.empty() == false)
				{
					m_freeQueryWithoutFilters += " ";
				}
				m_freeQueryWithoutFilters += term;
			}
			else
			{
				switch (term[0])
				{
					case 'L':
						m_language = term.substr(1);
						break;
					case 'H':
						m_hostFilter = term.substr(1);
						break;
					case 'P':
						m_fileFilter = term.substr(1);
						break;
					default:
						break;
				}
			}
		}
	}
}

/// Sets the name.
void QueryProperties::setName(const string &name)
{
	m_name = name;
}

/// Gets the name.
string QueryProperties::getName(void) const
{
	return m_name;
}

/// Sets the query string.
void QueryProperties::setFreeQuery(const string &freeQuery)
{
	m_freeQuery = freeQuery;
	removeFilters();
}

/// Gets the query string.
string QueryProperties::getFreeQuery(bool withoutFilters) const
{
	if (withoutFilters == false)
	{
		return m_freeQuery;
	}

#ifdef DEBUG
	cout << "QueryProperties::getFreeQuery: " << m_freeQueryWithoutFilters << endl;
#endif
	return m_freeQueryWithoutFilters;
}

/// Gets the query's language.
string QueryProperties::getLanguage(void) const
{
	return m_language;
}

/// Gets the query's host filter.
string QueryProperties::getHostFilter(void) const
{
	return m_hostFilter;
}

/// Gets the query's file filter.
string QueryProperties::getFileFilter(void) const
{
	return m_fileFilter;
}

/// Sets the maximum number of results.
void QueryProperties::setMaximumResultsCount(unsigned int count)
{
	m_resultsCount = count;
}

/// Gets the maximum number of results.
unsigned int QueryProperties::getMaximumResultsCount(void) const
{
	return m_resultsCount;
}

/// Sets whether results should be indexed.
void QueryProperties::setIndexResults(bool index)
{
	m_indexResults = index;
}

/// Gets whether results should be indexed
bool QueryProperties::getIndexResults(void) const
{
	return m_indexResults;
}

/// Sets the name of the label to use for indexed documents.
void QueryProperties::setLabelName(const string &labelName)
{
	m_labelName = labelName;
}

/// Gets the name of the label to use for indexed documents.
string QueryProperties::getLabelName(void) const
{
	return m_labelName;
}

/// Returns the query's terms.
void QueryProperties::getTerms(set<string> &terms) const
{
	Document doc;

	doc.setData(m_freeQueryWithoutFilters.c_str(), m_freeQueryWithoutFilters.length());
	Tokenizer tokens(&doc);

	terms.clear();

	string token;
	while (tokens.nextToken(token) == true)
	{
		terms.insert(token);
	}
}


/*
 *  Copyright 2008 Fabrice Colin
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
#include <stdarg.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>

#include "SQLDB.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

SQLRow::SQLRow(unsigned int nColumns) :
	m_nColumns(nColumns)
{
}

SQLRow::~SQLRow()
{
}

unsigned int SQLRow::getColumnsCount(void) const
{
	return m_nColumns;
}

SQLResults::SQLResults(unsigned long nRows, unsigned int nColumns) :
	m_nRows(nRows),
	m_nColumns(nColumns),
	m_nCurrentRow(0)
{
	// Check we actually have results
	if (m_nRows == 0)
	{
		m_nRows = m_nColumns = m_nCurrentRow = 0;
	}
}

SQLResults::~SQLResults()
{
}

bool SQLResults::hasMoreRows(void) const
{
	if ((m_nRows > 0) &&
		(m_nCurrentRow < m_nRows))
	{
		return true;
	}

	return false;
}

bool SQLResults::rewind(void)
{
	m_nCurrentRow = 0;

	return true;
}

SQLDB::SQLDB(const string &databaseName) :
	m_databaseName(databaseName)
{
}

SQLDB::~SQLDB()
{
}


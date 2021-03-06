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

#ifndef _SQL_DB_H
#define _SQL_DB_H

#include <string>

/// A row in a table.
class SQLRow
{
	public:
		virtual ~SQLRow();

		unsigned int getColumnsCount(void) const;

		virtual std::string getColumn(unsigned int nColumn) const = 0;

	protected:
		unsigned int m_nColumns;

		SQLRow(unsigned int nColumns);

};

/// Results extracted from the database.
class SQLResults
{
	public:
		virtual ~SQLResults();

		bool hasMoreRows(void) const;

		virtual std::string getColumnName(unsigned int nColumn) const = 0;

		virtual SQLRow *nextRow(void) = 0;

		virtual bool rewind(void);

	protected:
		unsigned long m_nRows;
		unsigned int m_nColumns;
		unsigned long int m_nCurrentRow;

		SQLResults(unsigned long nRows, unsigned int nColumns);

	private:
		SQLResults(const SQLResults &other);
		SQLResults &operator=(const SQLResults &other);

};

/// A SQL database.
class SQLDB
{
	public:
		virtual ~SQLDB();

		virtual bool isOpen(void) const = 0;

		virtual bool alterTable(const std::string &tableName,
			const std::string &columns,
			const std::string &newDefinition) = 0;

		virtual bool executeSimpleStatement(const std::string &sql) = 0;

		virtual SQLResults *executeStatement(const char *sqlFormat, ...) = 0;

	protected:
		std::string m_databaseName;

		SQLDB(const std::string &databaseName);

	private:
		SQLDB(const SQLDB &other);
		SQLDB &operator=(const SQLDB &other);

};

#endif // _SQL_DB_H

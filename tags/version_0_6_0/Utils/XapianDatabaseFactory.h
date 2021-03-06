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

#ifndef _XAPIAN_DATABASE_FACTORY_H
#define _XAPIAN_DATABASE_FACTORY_H

#include <string>
#include <map>
#include <pthread.h>

#include "XapianDatabase.h"

class XapianDatabaseFactory
{
	public:
		virtual ~XapianDatabaseFactory();

		/// Merges two databases together and add the result to the list.
		static bool mergeDatabases(const std::string &name,
			XapianDatabase *pFirst, XapianDatabase *pSecond);

		/// Returns a XapianDatabase pointer; NULL if unavailable.
		static XapianDatabase *getDatabase(const std::string &location, bool readOnly = true);

		/// Closes all databases.
		static void closeAll(void);

	protected:
		static pthread_mutex_t m_mutex;
		static std::map<std::string, XapianDatabase *> m_databases;

		XapianDatabaseFactory();

	private:
		XapianDatabaseFactory(const XapianDatabaseFactory &other);
		XapianDatabaseFactory &operator=(const XapianDatabaseFactory &other);

};

#endif // _XAPIAN_DATABASE_FACTORY_H

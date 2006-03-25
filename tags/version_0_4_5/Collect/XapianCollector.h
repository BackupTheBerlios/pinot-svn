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

#ifndef _XAPIAN_COLLECTOR_H
#define _XAPIAN_COLLECTOR_H

#include <string>

#include <xapian.h>

#include "DownloaderInterface.h"

class XapianCollector : public DownloaderInterface
{
	public:
		XapianCollector();
		XapianCollector(const std::string &database, bool getDocData);
		virtual ~XapianCollector();

		/// Retrieves the specified document; NULL if error. Caller deletes.
		virtual Document *retrieveUrl(const DocumentInfo &docInfo);

	protected:
		std::string m_databaseName;
		bool m_getDocData;
		Xapian::Database *m_pDatabase;

		bool openDatabase(void);

	private:
		XapianCollector(const XapianCollector &other);
		XapianCollector &operator=(const XapianCollector &other);

};

#endif // _XAPIAN_COLLECTOR_H

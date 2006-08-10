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
 
#ifndef _ONDISKHANDLER_HH
#define _ONDISKHANDLER_HH

#include <time.h>
#include <string>
#include <set>
#include <sigc++/slot.h>

#include "IndexedDocument.h"
#include "MboxParser.h"
#include "WritableXapianIndex.h"
#include "MonitorHandler.h"
#include "PinotSettings.h"

class OnDiskHandler : public MonitorHandler
{
	public:
		OnDiskHandler();
		virtual ~OnDiskHandler();

		/// Returns locations.
		virtual bool getLocations(std::set<std::string> &newLocations,
			std::set<std::string> &locationsToRemove);

		/// Handles file existence events.
		virtual bool fileExists(const std::string &fileName);

		/// Handles file creation events.
		virtual bool fileCreated(const std::string &fileName);

		/// Handles file modified events.
		virtual bool fileModified(const std::string &fileName);

		/// Handles file moved events.
		virtual bool fileMoved(const std::string &fileName,
			const std::string &previousFileName);

		/// Handles file deleted events.
		virtual bool fileDeleted(const std::string &fileName);

	protected:
		WritableXapianIndex m_index;
		std::set<std::string> m_locations;

		bool indexFile(const std::string &fileName, bool alwaysUpdate);

	private:
		OnDiskHandler(const OnDiskHandler &other);
		OnDiskHandler &operator=(const OnDiskHandler &other);

};

#endif // _ONDISKHANDLER_HH
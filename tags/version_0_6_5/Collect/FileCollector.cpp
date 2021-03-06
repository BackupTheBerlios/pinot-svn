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
#include <algorithm>

#include "MIMEScanner.h"
#include "Url.h"
#include "FileCollector.h"

using namespace std;

FileCollector::FileCollector() :
	DownloaderInterface()
{
}

FileCollector::~FileCollector()
{
}

//
// Implementation of DownloaderInterface
//

/// Retrieves the specified document; NULL if error.
Document *FileCollector::retrieveUrl(const DocumentInfo &docInfo)
{
	Url thisUrl(docInfo.getLocation());
	string protocol = thisUrl.getProtocol();

	if (protocol != "file")
	{
		// We can't handle that type of protocol...
		return NULL;
	}

	string directoryName = thisUrl.getLocation();
	string fileName = thisUrl.getFile();
	string fileLocation = directoryName;
	fileLocation += "/";
	fileLocation += fileName;

	Document *pDocument = new Document(docInfo);

	// Determine the file type
	pDocument->setType(MIMEScanner::scanFile(fileLocation));
	if (pDocument->setDataFromFile(fileLocation) == false)
	{
		delete pDocument;
		return NULL;
	}

	return pDocument;
}

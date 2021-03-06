/*
 *  Copyright 2007 Fabrice Colin
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

#include <time.h>
#include <iostream>

#include "Url.h"
#include "FilterFactory.h"
#include "TextFilter.h"
#include "FilterUtils.h"
#include "TextConverter.h"
#include "FilterWrapper.h"

using std::cout;
using std::endl;
using std::string;
using std::set;
using namespace Dijon;

static string convertToUTF8(const char *pData, unsigned int dataLen, const string &charset)
{
	TextConverter converter(20);

#ifdef DEBUG
	cout << "FilterWrapper::filterDocument: filter requested conversion from " << charset << endl;
#endif
	return converter.toUTF8(pData, dataLen, charset);
}

FilterWrapper::FilterWrapper(IndexInterface *pIndex) :
	m_pIndex(pIndex)
{
}

FilterWrapper::~FilterWrapper()
{
}

bool FilterWrapper::indexDocument(const Document &doc, const set<string> &labels, unsigned int &docId)
{
	string originalType(doc.getType());

	if (m_pIndex == NULL)
	{
		return false;
	}

	unindexNestedDocuments(doc.getLocation());

	return filterDocument(doc, originalType, labels, docId, false);
}

bool FilterWrapper::updateDocument(const Document &doc, unsigned int docId)
{
	set<string> labels;
	string originalType(doc.getType());

	if (m_pIndex == NULL)
	{
		return false;
	}

	unindexNestedDocuments(doc.getLocation());

	return filterDocument(doc, originalType, labels, docId, true);
}

bool FilterWrapper::unindexDocument(const string &location)
{
	if (m_pIndex == NULL)
	{
		return false;
	}

	unindexNestedDocuments(location);

	return m_pIndex->unindexDocument(location);
}

bool FilterWrapper::filterDocument(const Document &doc, const string &originalType,
	const set<string> &labels, unsigned int &docId, bool doUpdate)
{
	Filter *pFilter = FilterUtils::getFilter(doc.getType());
	bool fedFilter = false, docSuccess = false, finalSuccess = false;

	if (pFilter != NULL)
	{
		// The filter may have to convert the content to UTF-8 itself
    		pFilter->set_utf8_converter(convertToUTF8);

		fedFilter = FilterUtils::feedFilter(doc, pFilter);
	}
	else
	{
		// Chances are this type is not supported
		pFilter = new TextFilter("text/plain");

		Document emptyDoc(doc.getTitle(), doc.getLocation(), doc.getType(), doc.getLanguage());

		emptyDoc.setTimestamp(doc.getTimestamp());
		emptyDoc.setSize(doc.getSize());
		emptyDoc.setData(" ", 1);

#ifdef DEBUG
		cout << "FilterWrapper::filterDocument: unsupported type " << doc.getType() << endl;
#endif
		fedFilter = FilterUtils::feedFilter(emptyDoc, pFilter);
	}

	if (fedFilter == false)
	{
		delete pFilter;

		return false;
	}

	while (pFilter->has_documents() == true)
	{
		string actualType(originalType);
		bool isNested = false;
		bool emptyTitle = false;

		if (pFilter->next_document() == false)
		{
#ifdef DEBUG
			cout << "FilterWrapper::filterDocument: no more documents in " << doc.getLocation() << endl;
#endif
			break;
		}

		string originalTitle(doc.getTitle());
		Document filteredDoc(originalTitle, doc.getLocation(), "text/plain", doc.getLanguage());

		filteredDoc.setTimestamp(doc.getTimestamp());
		filteredDoc.setSize(doc.getSize());
		docSuccess = false;

		if (FilterUtils::populateDocument(filteredDoc, pFilter) == false)
		{
			continue;
		}

		// Is this a nested document ?
		if (filteredDoc.getLocation().length() > doc.getLocation().length())
		{
			actualType = filteredDoc.getType();
#ifdef DEBUG
			cout << "FilterWrapper::filterDocument: nested document of type " << actualType << endl;
#endif
			isNested = true;
		}
		else if (originalTitle.empty() == false)
		{
			// Preserve the top-level document's title
			filteredDoc.setTitle(originalTitle);
		}
		else if (filteredDoc.getTitle().empty() == true)
		{
			emptyTitle = true;
		}

		// Pass it down to another filter ?
		if ((filteredDoc.getType().length() >= 10) &&
			(filteredDoc.getType().substr(0, 10) == "text/plain"))
		{
			// Do we need to set a default title ?
			if (emptyTitle == true)
			{
				Url urlObj(doc.getLocation());

				// Default to the file name as title
				filteredDoc.setTitle(urlObj.getFile());
#ifdef DEBUG
				cout << "FilterWrapper::filterDocument: set default title " << urlObj.getFile() << endl;
#endif
			}

			// No, it's been reduced to plain text
			filteredDoc.setType(actualType);

			// Nested documents can't be updated because they are unindexed
			// and the ID is that of the base document anyway
			if ((doUpdate == true) &&
				(isNested == false))
			{
				docSuccess = m_pIndex->updateDocument(docId, filteredDoc);
			}
			else
			{
				unsigned int newDocId = docId;

				docSuccess = m_pIndex->indexDocument(filteredDoc, labels, newDocId);
				// Make sure we return the base document's ID, not the last nested document's ID
				if (isNested == false)
				{
					docId = newDocId;
				}
			}
		}
		else
		{
			docSuccess = filterDocument(filteredDoc, actualType, labels, docId, doUpdate);
		}

		// Consider indexing anything a success
		if (docSuccess == true)
		{
			finalSuccess = true;
		}
	}

	delete pFilter;

#ifdef DEBUG
	cout << "FilterWrapper::filterDocument: done with " << doc.getLocation() << " status " << finalSuccess << endl;
#endif

	return finalSuccess;
}

bool FilterWrapper::unindexNestedDocuments(const string &url)
{
	// Unindex all documents that stem from this file
	return m_pIndex->unindexDocuments(url, IndexInterface::BY_FILE);
}

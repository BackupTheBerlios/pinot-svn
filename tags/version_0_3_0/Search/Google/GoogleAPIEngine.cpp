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

#include "Url.h"
#include "MIMEScanner.h"

#include "GoogleAPIEngine.h"
#include "GAPIGoogleSearchBindingProxy.h"
#include "GAPI.nsmap"

using namespace GAPI;

GoogleAPIEngine::GoogleAPIEngine() :
	SearchEngineInterface()
{
	// SearchEngineInterface members
	m_maxResultsCount = 10;
}

GoogleAPIEngine::~GoogleAPIEngine()
{
	m_resultsList.clear();
}

/// Retrieves the specified URL from the cache; NULL if error. Caller deletes.
Document *GoogleAPIEngine::retrieveCachedUrl(const string &url)
{
	GoogleSearchBinding soapProxy;
	struct gapi1__doGetCachedPageResponse cacheOut;

	if (soapProxy.gapi1__doGetCachedPage(m_key, url, cacheOut))
	{
		return NULL;
	}

	xsd__base64Binary base64Page = cacheOut.return_;
	if ((base64Page.__ptr != NULL) &&
		(base64Page. __size > 0))
	{
		Url urlObj(url);

		Document *pDoc = new Document(url, url, MIMEScanner::scanUrl(urlObj), "");
		pDoc->setData((const char*)base64Page.__ptr, (unsigned int)base64Page. __size);

		return pDoc;
	}
	
	return NULL;
}

/// Checks spelling.
string GoogleAPIEngine::checkSpelling(const string &text)
{
	GoogleSearchBinding soapProxy;
	struct gapi1__doSpellingSuggestionResponse spellOut;

	if (soapProxy.gapi1__doSpellingSuggestion(m_key, text, spellOut))
	{
		return "";
	}

	return spellOut.return_;
}

//
// Implementation of SearchEngineInterface
//

/// Runs a query; true if success.
bool GoogleAPIEngine::runQuery(QueryProperties& queryProps)
{
	string andTerms = queryProps.getAndWords();
	string phrase = queryProps.getPhrase();

	setHostNameFilter(queryProps.getHostNameFilter());
	setFileNameFilter(queryProps.getFileNameFilter());

	if (m_key.empty() == true)
	{
		return false;
	}

	// FIXME: find out how m_notWords and m_anyWords could be used
	if (andTerms.empty() == true)
	{
		if (phrase.empty() == true)
		{
			return false;
		}
		// Use the phrase as search terms then...
		andTerms = phrase;
		phrase = "";
	}

	GoogleSearchBinding soapProxy;
	struct gapi1__doGoogleSearchResponse queryOut;

	// No filter, no safe search
	if (soapProxy.gapi1__doGoogleSearch(m_key, andTerms, 0, (m_maxResultsCount > 10 ? 10 : m_maxResultsCount),
		((phrase.empty() == false) ? true : false), phrase, false, "", "latin1", "latin1", queryOut))
	{
		return false;
	}

	struct gapi1__GoogleSearchResult *searchResult = queryOut.return_;
	float pseudoScore = 100;

	for (int i = 0; i < searchResult->resultElements->__size; i++)
	{
		struct gapi1__ResultElement *resultElement = searchResult->resultElements->__ptr[i];

		string resultUrl(resultElement->URL);
		if (processResult(resultUrl) == true)
		{
			m_resultsList.push_back(Result(resultUrl, resultElement->title, resultElement->snippet, "", pseudoScore));
			--pseudoScore;
		}
	}

	return true;
}

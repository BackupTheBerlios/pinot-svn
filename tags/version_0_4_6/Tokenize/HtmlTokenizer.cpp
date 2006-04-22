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

#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <regex.h>
#include <stack>
#include <iostream>
#include <algorithm>

#include "StringManip.h"
#include "HtmlTokenizer.h"

//#define DEBUG_TOKENIZER

using std::cout;
using std::endl;
using std::string;
using std::map;
using std::set;
using std::stack;

HtmlTokenizer::HtmlTokenizer(const Document *pDocument, unsigned int linksStartAtPos) :
	Tokenizer(NULL),
	m_pHtmlDocument(NULL),
	m_linkPos(linksStartAtPos)
{
	initialize(pDocument);
}

HtmlTokenizer::HtmlTokenizer() :
	Tokenizer(NULL),
	m_pHtmlDocument(NULL),
	m_linkPos(0)
{
}

HtmlTokenizer::~HtmlTokenizer()
{
	if (m_pDocument != NULL)
	{
		// This should have been set by setDocument(),
		// called in initialize()
		delete m_pDocument;
	}
}

void HtmlTokenizer::initialize(const Document *pDocument)
{
	unsigned int length = 0;

	if (pDocument == NULL)
	{
		return;
	}

	const char *data = pDocument->getData(length);
	if ((data != NULL) &&
		(length > 0))
	{
		// Remove HTML tags
		string strippedData = parseHTML(data);
		// Append META keywords, if any were found
		strippedData += getMetaTag("keywords");

		// Pass the result to the parent class
		Document *pStrippedDoc = new Document(pDocument->getTitle(),
			pDocument->getLocation(), pDocument->getType(),
			pDocument->getLanguage());
		pStrippedDoc->setData(strippedData.c_str(), strippedData.length());
		setDocument(pStrippedDoc);

		m_pHtmlDocument = pDocument;
	}
}

/// Parses HTML; the string without tags.
string HtmlTokenizer::parseHTML(const string &str, bool stripAllBlocks)
{
	stack<string> tagsStack;
	string stripped, link, linkName;
	string::size_type startPos = 0, linkOpenPos = 0;
	regex_t linksRegex, metaRegex;
	bool isHtml = false, skip = false, catText = stripAllBlocks;
	bool extractLinks = true, extractMetaTags = true, getLinkName = false;

#ifdef DEBUG_TOKENIZER
	cout << "HtmlTokenizer::parseHTML: start" << endl;
#endif
	// Prepare the regexps
	if (regcomp(&linksRegex, "a(.*)href=(.*)", REG_EXTENDED|REG_ICASE) != 0)
	{
#ifdef DEBUG_TOKENIZER
		cout << "HtmlTokenizer::parseHTML: couldn't compile links regexp" << endl;
#endif
		extractLinks = false;
	}
	if (regcomp(&metaRegex, "meta name=(.*) content=(.*)", REG_EXTENDED|REG_ICASE) != 0)
	{
#ifdef DEBUG_TOKENIZER
		cout << "HtmlTokenizer::parseHTML: couldn't compile meta tag regexp" << endl;
#endif
		extractMetaTags = false;
	}

	// Tag start
	string::size_type pos = str.find("<");
	while (pos != string::npos)
	{
		isHtml = true;

		if (skip == false)
		{
			string text = str.substr(startPos, pos - startPos);
			if (catText == true)
			{
				stripped += StringManip::replaceEntities(text);
				stripped += " ";
			}

			// Is this part of the name of the last link we found ?
			if (getLinkName == true)
			{
				linkName += text;
#ifdef DEBUG_TOKENIZER
				cout << "HtmlTokenizer::parseHTML: adding to name " << text << endl;
#endif
			}

			startPos = pos + 1;
			// Tag end
			if (str[pos] == '<')
			{
				pos = str.find(">", startPos);
			}
			// Skip stuff in the tag
			skip = true;
		}
		else
		{
			regmatch_t pLinksMatches[3];
			regmatch_t pMetaMatches[3];
			int nLinksMatches = 3, nMetaMatches = 3;

			// Found a tag from startPos to pos
			string tag(str.substr(startPos, pos - startPos));
			// Push it onto the stack
			tagsStack.push(tag);
#ifdef DEBUG_TOKENIZER
			cout << "HtmlTokenizer::parseHTML: found " << tag << endl;
#endif

			if ((extractMetaTags == true) &&
				(regexec(&metaRegex, tag.c_str(), nMetaMatches, pMetaMatches, 
					REG_NOTBOL|REG_NOTEOL) == 0) &&
				(pMetaMatches[nMetaMatches - 1].rm_so != -1))
			{
				string tmp, metaName, metaContent;

				// META tag name
				tmp = tag.substr(pMetaMatches[1].rm_so,
					pMetaMatches[1].rm_eo - pMetaMatches[1].rm_so);
				// Remove quotes
				metaName = StringManip::removeQuotes(tmp);

				// META tag content
				tmp = tag.substr(pMetaMatches[2].rm_so,
					pMetaMatches[2].rm_eo - pMetaMatches[2].rm_so);
				// Remove quotes
				metaContent = StringManip::removeQuotes(tmp);
#ifdef DEBUG_TOKENIZER
				cout << "HtmlTokenizer::parseHTML: found META tag " << metaName << ": " << metaContent << endl;
#endif
				m_metaTags[metaName] = metaContent;
			}
			// See if this tag is an anchor
			// pLinksMatches[0] will be something like 'a href="blah"', pLinksMatches[1] will be ' ' and [2] will be '"blah"'
			else if ((extractLinks == true) &&
				(regexec(&linksRegex, tag.c_str(), nLinksMatches, pLinksMatches, REG_NOTBOL|REG_NOTEOL) == 0) &&
				(pLinksMatches[nLinksMatches - 1].rm_so != -1))
			{
				string quotedLink = tag.substr(pLinksMatches[2].rm_so,
					pLinksMatches[2].rm_eo - pLinksMatches[2].rm_so);

#ifdef DEBUG_TOKENIZER
				cout << "HtmlTokenizer::parseHTML: found link start " << tag << endl;
#endif
				if (link.empty() == false)
				{
					// The previous link's anchor's end couldn't be found ?
					m_links.insert(Link(stripTags(link), linkName, m_linkPos, linkOpenPos, startPos - 1));
					m_linkPos++;
					linkName = "";
#ifdef DEBUG_TOKENIZER
					cout << "HtmlTokenizer::parseHTML: previous link wasn't closed properly" << endl;
#endif
				}

				// Remove quotes
				link = StringManip::removeQuotes(quotedLink);
				linkOpenPos = startPos - 1;

				// Remember to get the name of the link
				getLinkName = true;
			}
			// Maybe it's the anchor's end ?
			else if ((extractLinks == true) &&
				(strncasecmp(tag.c_str(), "/a", 2) == 0))
			{
				if (getLinkName == true)
				{
#ifdef DEBUG_TOKENIZER
					cout << "HtmlTokenizer::parseHTML: " << pos << " link " << m_linkPos << " is " << link << ", name " << linkName << endl;
#endif
					// New link
					m_links.insert(Link(stripTags(link), linkName, m_linkPos, linkOpenPos, pos + 1));
					m_linkPos++;
					getLinkName = false;
					link = linkName = "";
				}
			}
			else if (stripAllBlocks == false)
			{
				if (textBlockStart(tag) == true)
				{
					catText = true;
#ifdef DEBUG_TOKENIZER
					cout << "HtmlTokenizer::parseHTML: start text cat" << endl;
#endif
				}
				else if (textBlockEnd(tag) == true)
				{
					catText = false;
#ifdef DEBUG_TOKENIZER
					cout << "HtmlTokenizer::parseHTML: end text cat" << endl;
#endif
				}
				else
				{
					string parentTag = tagsStack.top();

					if ((strncasecmp(tag.c_str(), "script", 6) == 0) ||
						(strncasecmp(tag.c_str(), "style", 5) == 0))
					{
#ifdef DEBUG_TOKENIZER
						cout << "HtmlTokenizer::parseHTML: skip script" << endl;
#endif
						catText = false;
					}
					else if (((strncasecmp(tag.c_str(), "/script", 7) == 0) ||
						(strncasecmp(tag.c_str(), "/style", 6) == 0)) &&
						(textBlockStart(parentTag) == false))
					{
#ifdef DEBUG_TOKENIZER
						cout << "HtmlTokenizer::parseHTML: stop skip script" << endl;
#endif
						catText = true;
					}
				}
			}

			startPos = pos + 1;
			pos = str.find("<", startPos);
			skip = false;
		}
	}
	if ((startPos < str.length()) &&
		(catText == true))
	{
		stripped  += StringManip::replaceEntities(str.substr(startPos));
	}

	// Free the compiled regexps
	regfree(&linksRegex);
	regfree(&metaRegex);

	if (isHtml == false)
	{
		return str;
	}

	return stripped;
}

/// Returns true if the tag corresponds to a text block.
bool HtmlTokenizer::textBlockStart(const string &tag)
{
	if ((strncasecmp(tag.c_str(), "body", 4) == 0) ||
		(strncasecmp(tag.c_str(), "title", 5) == 0))
	{
		return true;
	}

	return false;
}

/// Returns true if the tag corresponds to the end of a text block.
bool HtmlTokenizer::textBlockEnd(const string &tag)
{
	if ((strncasecmp(tag.c_str(), "/body", 5) == 0) ||
		(strncasecmp(tag.c_str(), "/title", 6) == 0))
	{
		return true;
	}

	return false;
}

/// Gets the specified META tag content.
string HtmlTokenizer::getMetaTag(const string &name)
{
	if (name.empty() == true)
	{
		return "";
	}

	map<string, string>::const_iterator iter = m_metaTags.find(name);
	if (iter != m_metaTags.end())
	{
		return iter->second;
	}

	return "";
}

/// Gets the links map.
set<Link> &HtmlTokenizer::getLinks(void)
{
	return m_links;
}

/// Utility method that strips HTML tags off; the string without tags.
string HtmlTokenizer::stripTags(const string &str)
{
	HtmlTokenizer tokens;

	return tokens.parseHTML(str, true);
}

Link::Link(const string &url, const string &name, unsigned int pos, unsigned int openPos, unsigned int closePos) :
	m_url(url),
	m_name(name),
	m_pos(pos),
	m_open(openPos),
	m_close(closePos)
{
}

Link::Link(const Link &other) :
	m_url(other.m_url),
	m_name(other.m_name),
	m_pos(other.m_pos),
	m_open(other.m_open),
	m_close(other.m_close)
{
}

Link::~Link()
{
}

Link& Link::operator=(const Link& other)
{
	m_url = other.m_url;
	m_name = other.m_name;
	m_pos = other.m_pos;
	m_open = other.m_open;
	m_close = other.m_close;

	return *this;
}

bool Link::operator==(const Link &other) const
{
	return m_url == other.m_url;
}

bool Link::operator<(const Link &other) const
{
	return m_pos < other.m_pos;
}

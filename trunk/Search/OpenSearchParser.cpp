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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <glibmm/thread.h>
#include <glibmm/convert.h>
#include <libxml/parserInternals.h>
#include <libxml++/parsers/domparser.h>
#include <libxml++/nodes/node.h>
#include <libxml++/nodes/textnode.h>

#include "StringManip.h"
#include "OpenSearchParser.h"

using namespace std;
using namespace Glib;
using namespace xmlpp;

static ustring getElementContent(const Element *pElem)
{
	if (pElem == NULL)
	{
		return "";
	}

#ifdef HAS_LIBXMLPP026
	const TextNode *pText = pElem->get_child_content();
#else
	const TextNode *pText = pElem->get_child_text();
#endif
	if (pText == NULL)
	{
		return "";
	}

	return pText->get_content();
}

OpenSearchResponseParser::OpenSearchResponseParser(bool rssResponse) :
	ResponseParserInterface(),
	m_rssResponse(rssResponse)
{
}

OpenSearchResponseParser::~OpenSearchResponseParser()
{
}

bool OpenSearchResponseParser::parse(const ::Document *pResponseDoc, vector<Result> &resultsList,
	unsigned int maxResultsCount) const
{
	float pseudoScore = 100;
	unsigned int contentLen = 0;
	bool success = true;

	if ((pResponseDoc == NULL) ||
		(pResponseDoc->getData(contentLen) == NULL) ||
		(contentLen == 0))
	{
		return false;
	}

	const char *pContent = pResponseDoc->getData(contentLen);
	try
	{
		bool loadFeed = false;

		// Parse the configuration file
		DomParser parser;
		parser.set_substitute_entities(true);
		parser.parse_memory_raw((const unsigned char *)pContent, (Parser::size_type)contentLen);
		xmlpp::Document *pDocument = parser.get_document();
		if (pDocument == NULL)
		{
			return false;
		}

		Node *pNode = pDocument->get_root_node();
		Element *pRootElem = dynamic_cast<Element *>(pNode);
		if (pRootElem == NULL)
		{
			return false;
		}
		// Check the top-level element is what we expect
		ustring rootNodeName = pRootElem->get_name();
		if (m_rssResponse == true)
		{
			if (rootNodeName == "rss")
			{
				const Node::NodeList rssChildNodes = pRootElem->get_children();
				for (Node::NodeList::const_iterator rssIter = rssChildNodes.begin();
					rssIter != rssChildNodes.end(); ++rssIter)
				{
					Node *pRssNode = (*rssIter);
					Element *pRssElem = dynamic_cast<Element*>(pRssNode);
					if (pRssElem != NULL)
					{
						if (pRssElem->get_name() == "channel")
						{
							pRootElem = pRssElem;
							loadFeed = true;
							break;
						}
					}
				}
			}
		}
		else
		{
			if (rootNodeName != "feed")
			{
				return false;
			}
			loadFeed = true;
		}

		if (loadFeed == false)
		{
#ifdef DEBUG
			cout << "OpenSearchResponseParser::parse: error on root node "
				<< rootNodeName << endl;
#endif
			return false;
		}

		// RSS
		ustring itemNode("item");
		ustring descriptionNode("description");
		if (m_rssResponse == false)
		{
			// Atom
			itemNode = "entry";
			descriptionNode = "content";
		}

		// Go through the subnodes
		const Node::NodeList childNodes = pRootElem->get_children();
		for (Node::NodeList::const_iterator iter = childNodes.begin();
			iter != childNodes.end(); ++iter)
		{
			Node *pNode = (*iter);
			// All nodes should be elements
			Element *pElem = dynamic_cast<Element*>(pNode);
			if (pElem == NULL)
			{
				continue;
			}

			ustring nodeName = pElem->get_name();
			if (nodeName != itemNode)
			{
				continue;
			}

			// Go through the item's subnodes
			ustring title, url, extract;
			const Node::NodeList itemChildNodes = pElem->get_children();
			for (Node::NodeList::const_iterator itemIter = itemChildNodes.begin();
				itemIter != itemChildNodes.end(); ++itemIter)
			{
				Node *pItemNode = (*itemIter);
				// All nodes should be elements
				Element *pItemElem = dynamic_cast<Element*>(pItemNode);
				if (pItemElem == NULL)
				{
					continue;
				}

				ustring itemNodeName = pItemElem->get_name();
				if (itemNodeName == "title")
				{
					title = getElementContent(pItemElem);
				}
				else if (itemNodeName == "link")
				{
					if (m_rssResponse == true)
					{
						url = getElementContent(pItemElem);
					}
					else
					{
						Attribute *pAttr = pItemElem->get_attribute("href");
						if (pAttr != NULL)
						{
							url = pAttr->get_value();
						}
					}
				}
				else if (itemNodeName == descriptionNode)
				{
					extract = getElementContent(pItemElem);
				}
			}

			resultsList.push_back(Result(url, title, extract, "", pseudoScore));
			--pseudoScore;
			success = true;
		}
	}
	catch (const std::exception& ex)
	{
#ifdef DEBUG
		cout << "OpenSearchResponseParser::parse: caught exception: " << ex.what() << endl;
#endif
		success = false;
	}

	return success;
}

OpenSearchParser::OpenSearchParser(const string &fileName) :
	PluginParserInterface(fileName)
{
}

OpenSearchParser::~OpenSearchParser()
{
}

ResponseParserInterface *OpenSearchParser::parse(SearchPluginProperties &properties,
	bool extractSearchParams)
{
	struct stat fileStat;
	bool rssResponse = true, success = true;

	if ((m_fileName.empty() == true) ||
		(stat(m_fileName.c_str(), &fileStat) != 0) ||
		(!S_ISREG(fileStat.st_mode)))
	{
		return NULL;
	}

	try
	{
		// Parse the configuration file
		DomParser parser;
		parser.set_substitute_entities(true);
		parser.parse_file(m_fileName);
		xmlpp::Document *pDocument = parser.get_document();
		if (pDocument == NULL)
		{
			return NULL;
		}

		Node *pNode = pDocument->get_root_node();
		Element *pRootElem = dynamic_cast<Element *>(pNode);
		if (pRootElem == NULL)
		{
			return NULL;
		}
		// Check the top-level element is what we expect
		ustring rootNodeName = pRootElem->get_name();
		if (rootNodeName != "OpenSearchDescription")
		{
#ifdef DEBUG
			cout << "OpenSearchParser::parse: wrong root node " << rootNodeName << endl;
#endif
			return NULL;
		}

		// Go through the subnodes
		const Node::NodeList childNodes = pRootElem->get_children();
		if (childNodes.empty() == false)
		{
			for (Node::NodeList::const_iterator iter = childNodes.begin(); iter != childNodes.end(); ++iter)
			{
				Node *pNode = (*iter);
				// All nodes should be elements
				Element *pElem = dynamic_cast<Element*>(pNode);
				if (pElem == NULL)
				{
					continue;
				}

				ustring nodeName = pElem->get_name();
				ustring nodeContent = getElementContent(pElem);
				if (nodeName == "ShortName")
				{
					properties.m_name = nodeContent;
				}
				else if (nodeName == "Description")
				{
					properties.m_description = nodeContent;
				}
				else if (nodeName == "Url")
				{
					ustring url, type;
					SearchPluginProperties::Response response = SearchPluginProperties::RSS_RESPONSE;
					bool getMethod = true;

					// Parse Query Syntax
					Element::AttributeList attributes = pElem->get_attributes();
					for (Element::AttributeList::const_iterator iter = attributes.begin();
						iter != attributes.end(); ++iter)
					{
						Attribute *pAttr = (*iter);

						if (pAttr != NULL)
						{
							ustring attrName = pAttr->get_name();
							ustring attrContent = pAttr->get_value();

							if (attrName == "template")
							{
								url = attrContent;
							}
							else if (attrName == "type")
							{
								type = attrContent;
							}
							else if (attrName == "method")
							{
								// GET is the default method
								if (StringManip::toLowerCase(attrContent) != "get")
								{
									getMethod = false;
								}
							}
						}
					}

					// Did we get the URL ?
					if (url.empty() == true)
					{
						// It's probably provided as content, v1.0 style
						url = nodeContent;
					}

					if (getMethod == true)
					{
						string::size_type startPos = 0, pos = url.find("?");

						// Do we support that type ?
						if (type == "application/atom+xml")
						{
							response = SearchPluginProperties::ATOM_RESPONSE;
							rssResponse = false;
						}
						else if ((type.empty() == false) &&
							(type != "application/rss+xml"))
						{
#ifdef DEBUG
							cout << "OpenSearchParser::parse: unsupported response type "
								<< type << endl;
#endif
							continue;
						}
	
						// Break the URL down into base and parameters
						if (pos != string::npos)
						{
							string params(url.substr(pos + 1));

							// URL
							properties.m_baseUrl = url.substr(0, pos);
#ifdef DEBUG
							cout << "OpenSearchParser::parse: URL is " << url << endl;
#endif

							// Split this into the actual parameters
							params += "&";
							pos = params.find("&");
							while (pos != string::npos)
							{
								string parameter(params.substr(startPos, pos - startPos));

								string::size_type equalPos = parameter.find("=");
								if (equalPos != string::npos)
								{
									string paramName(parameter.substr(0, equalPos));
									string paramValue(parameter.substr(equalPos + 1));
									SearchPluginProperties::Parameter param = SearchPluginProperties::UNKNOWN_PARAM;

									if (paramValue == "{searchTerms}")
									{
										param = SearchPluginProperties::SEARCH_TERMS_PARAM;
									}
									else if (paramValue == "{count}")
									{
										param = SearchPluginProperties::COUNT_PARAM;
									}
									else if (paramValue == "{startIndex}")
									{
										param = SearchPluginProperties::START_INDEX_PARAM;
									}
									else if (paramValue == "{startPage}")
									{
										param = SearchPluginProperties::START_PAGE_PARAM;
									}
									else if (paramValue == "{language}")
									{
										param = SearchPluginProperties::LANGUAGE_PARAM;
									}
									else if (paramValue == "{outputEncoding}")
									{
										param = SearchPluginProperties::OUTPUT_ENCODING_PARAM;
									}
									else if (paramValue == "{inputEncoding}")
									{
										param = SearchPluginProperties::INPUT_ENCODING_PARAM;
									}

									if (param != SearchPluginProperties::UNKNOWN_PARAM)
									{
										properties.m_parameters[param] = paramName;
									}
									else
									{
										// Append to the remainder
										if (properties.m_parametersRemainder.empty() == false)
										{
											properties.m_parametersRemainder += "&";
										}
										properties.m_parametersRemainder += paramName;
										properties.m_parametersRemainder += "=";
										properties.m_parametersRemainder += paramValue;
									}
								}

								// Next
								startPos = pos + 1;
								pos = params.find_first_of("&", startPos);
							}
						}

						// Method
						properties.m_method = SearchPluginProperties::GET_METHOD;
						// Output type
						properties.m_outputType = type;
						// Response
						properties.m_response = response;
					}

					// We ignore Param as we only support GET
				}
				else if (nodeName == "Tags")
				{
					// This is a space-delimited list, pick the first tag
					string::size_type pos = nodeContent.find(" ");
					properties.m_channel = nodeContent.substr(0, pos);
				}
				else if (nodeName == "LongName")
				{
					properties.m_longName = nodeContent;
				}
				else if (nodeName == "Language")
				{
					properties.m_languages.insert(nodeContent);
				}
				else if (nodeName == "OutputEncoding")
				{
					properties.m_outputEncodings.insert(nodeContent);
				}
				else if (nodeName == "InputEncoding")
				{
					properties.m_inputEncodings.insert(nodeContent);
				}
			}
		}
	}
	catch (const std::exception& ex)
	{
#ifdef DEBUG
		cout << "OpenSearchParser::parse: caught exception: " << ex.what() << endl;
#endif
		success = false;
	}

	if (success == false)
	{
		return NULL;
	}

	return new OpenSearchResponseParser(rssResponse);
}

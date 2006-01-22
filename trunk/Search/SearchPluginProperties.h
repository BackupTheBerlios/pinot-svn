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

#ifndef _SEARCH_PLUGIN_PROPERTIES_H
#define _SEARCH_PLUGIN_PROPERTIES_H

#include <string>
#include <map>
#include <set>

class SearchPluginProperties
{
	public:
		SearchPluginProperties();
		SearchPluginProperties(const SearchPluginProperties &other);
		virtual ~SearchPluginProperties();

		SearchPluginProperties& operator=(const SearchPluginProperties& other);
		bool operator==(const SearchPluginProperties &other) const;
		bool operator<(const SearchPluginProperties &other) const;

		typedef enum { GET_METHOD = 0, POST_METHOD } Method;

		typedef enum { UNKNOWN_PARAM = 0, SEARCH_TERMS_PARAM,
			 COUNT_PARAM,START_INDEX_PARAM, START_PAGE_PARAM, LANGUAGE_PARAM,
			OUTPUT_ENCODING_PARAM, INPUT_ENCODING_PARAM } Parameter;

		typedef enum { UNKNOWN_RESPONSE = 0, XML_RESPONSE,
			HTML_RESPONSE } Response;

		// Description
		std::string m_name;
		std::string m_longName;
		std::string m_description;
		std::string m_channel;
		std::set<std::string> m_languages;
		std::set<std::string> m_outputEncodings;
		std::set<std::string> m_inputEncodings;
		// Query
		std::string m_baseUrl;
		Method m_method;
		std::map<Parameter, std::string> m_parameters;
		std::string m_parametersRemainder;
		std::string m_outputType;
		// Query, next page
		std::string m_nextTag;
		unsigned int m_nextFactor;
		unsigned int m_nextBase;
		// Response
		Response m_response;
		std::string m_resultListStart;
		std::string m_resultListEnd;
		std::string m_resultItemStart;
		std::string m_resultItemEnd;
		std::string m_resultTitleStart;
		std::string m_resultTitleEnd;
		std::string m_resultLinkStart;
		std::string m_resultLinkEnd;
		std::string m_resultExtractStart;
		std::string m_resultExtractEnd;
		bool m_skipLocal;

};

#endif // _SEARCH_PLUGIN_PROPERTIES_H

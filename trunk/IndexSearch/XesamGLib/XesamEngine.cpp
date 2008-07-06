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

#include <glib.h>
#include <xesam-glib/xesam-glib.h>
#include <vector>
#include <iostream>

#include "DocumentInfo.h"
#include "StringManip.h"
#include "XesamEngine.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;

class CallbackData
{
	public:
		CallbackData(GMainLoop *pMainLoop,
			vector<DocumentInfo> &resultsList,
			unsigned int &resultsCountEstimate) :
			m_pMainLoop(pMainLoop),
			m_resultsList(resultsList),
			m_resultsCountEstimate(resultsCountEstimate),
			m_gettingHits(false)
		{
		}
		~CallbackData()
		{
		}

		GMainLoop *m_pMainLoop;
		vector<DocumentInfo> &m_resultsList;
		unsigned int &m_resultsCountEstimate;
		bool m_gettingHits;

};

static void hitsReady(XesamGSearch *pSearch, XesamGHits *pHits, gpointer pUserData)
{
	if ((pHits == NULL) ||
		(pUserData == NULL))
	{
		return;
	}

	CallbackData *pData = (CallbackData *)pUserData;
	pData->m_gettingHits = true;

	for (guint i = 0; i < xesam_g_hits_get_count(pHits); ++i)
	{
		XesamGHit *pHit = xesam_g_hits_get(pHits, i);

		if (pHit == NULL)
		{
			continue;
		}
#ifdef DEBUG
		cout << "XesamEngine::runQuery: hit ID " << xesam_g_hit_get_id(pHit)
			<< ", URL " << xesam_g_hit_get_string(pHit, "xesam:url") << endl;
#endif

		// FIXME: push into pData->m_resultsList
	}

	pData->m_gettingHits = false;
}

static void searchDone(XesamGSearch *pSearch, gpointer pUserData)
{
	if (pUserData == NULL)
	{
		return;
	}

	CallbackData *pData = (CallbackData *)pUserData;

	// Check we are not in the middle of retrieving stuff
	while (pData->m_gettingHits == true);
	g_main_loop_quit(pData->m_pMainLoop);
}

XesamEngine::XesamEngine(const string &dbusObject) :
	SearchEngineInterface(),
	m_dbusObject(dbusObject)
{
}

XesamEngine::~XesamEngine()
{
}

bool XesamEngine::runQuery(QueryProperties& queryProps,
	unsigned int startDoc)
{
	string freeQuery(StringManip::replaceSubString(queryProps.getFreeQuery(), "\n", " "));
	QueryProperties::QueryType type = queryProps.getType();
	bool ranQuery = false;

        m_resultsList.clear();
        m_resultsCountEstimate = 0;

	// FIXME: creating a main loop here might not be such a hot idea...
	g_type_init();

	// Was an object name specified ?
	XesamGSearcher *pSearcher = NULL;
	if (m_dbusObject.empty() == false)
	{
		pSearcher = XESAM_G_SEARCHER(xesam_g_dbus_searcher_new(XESAM_G_SERVER_DBUS_NAME, m_dbusObject.c_str()));
	}
	else
	{
		pSearcher = XESAM_G_SEARCHER(xesam_g_dbus_searcher_new_default());
	}
	if (pSearcher == NULL)
	{
		return false;
	}

	XesamGSession *pSession = xesam_g_session_new(pSearcher);
	if (pSession == NULL)
	{
		g_object_unref(pSearcher);
		return false;
	}
	// FIXME: set session properties here

        // What type of query is this ?
	XesamGQuery *pQuery = NULL;
	if (type == QueryProperties::XESAM_QL)
	{
		pQuery = xesam_g_query_new_from_text(queryProps.getFreeQuery().c_str());
	}
	else if (type == QueryProperties::XESAM_UL)
	{
		pQuery = xesam_g_query_new_from_xml(queryProps.getFreeQuery().c_str());
	}
	if (pQuery == NULL)
	{
		g_object_unref(pSession);
		g_object_unref(pSearcher);
		return false;
	}

	XesamGSearch *pSearch = xesam_g_session_new_search(pSession, pQuery);
	if (pSearch == NULL)
	{
		g_object_unref(pQuery);
		g_object_unref(pSession);
		g_object_unref(pSearcher);
		return false;
	}

	GMainLoop *pMainLoop = g_main_loop_new(NULL, FALSE);
	if (pMainLoop != NULL)
	{
		CallbackData *pData = new CallbackData(pMainLoop, m_resultsList, m_resultsCountEstimate);

		g_signal_connect(pSearch, "hits-ready", G_CALLBACK(hitsReady), pData);
		g_signal_connect(pSearch, "done", G_CALLBACK(searchDone), pData);

		// Start
		xesam_g_search_start(pSearch);
		g_main_loop_run(pMainLoop);

		// Stop
		xesam_g_search_close(pSearch);

		m_resultsCountEstimate = m_resultsList.size();

		delete pData;
		g_main_loop_unref(pMainLoop);
		ranQuery = true;
	}

	// Unreference all objects
	g_object_unref(pSearch);
	g_object_unref(pQuery);
	g_object_unref(pSession);
	g_object_unref(pSearcher);

	return ranQuery;
}

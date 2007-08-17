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

#include <ctype.h>
#include <iostream>
#include <gdkmm/color.h>
#include <gtkmm/alignment.h>
#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/stock.h>
#include <gtkmm/cellrendererprogress.h>

#include "StringManip.h"
#include "TimeConverter.h"
#include "Url.h"
#include "QueryHistory.h"
#include "ResultsExporter.h"
#include "ViewHistory.h"
#include "config.h"
#include "NLS.h"
#include "PinotSettings.h"
#include "PinotUtils.h"
#include "ResultsTree.h"

using namespace std;
using namespace SigC;
using namespace Glib;
using namespace Gdk;
using namespace Gtk;

ResultsTree::ResultsTree(const ustring &queryName, Menu *pPopupMenu,
	GroupByMode groupMode, PinotSettings &settings) :
	TreeView(),
	m_treeName(queryName),
	m_pPopupMenu(pPopupMenu),
	m_pResultsScrolledwindow(NULL),
	m_settings(settings),
	m_pExtractScrolledwindow(NULL),
	m_extractTreeView(NULL),
	m_showExtract(true),
	m_groupMode(groupMode)
{
	TreeViewColumn *pColumn = NULL;

	m_pResultsScrolledwindow = manage(new ScrolledWindow());
	m_pExtractScrolledwindow = manage(new ScrolledWindow());
	m_extractTreeView = manage(new TreeView());

	// This is the actual results tree
	set_events(Gdk::BUTTON_PRESS_MASK);
	set_flags(CAN_FOCUS);
	set_headers_clickable(true);
	set_headers_visible(true);
	set_rules_hint(true);
	set_reorderable(false);
	set_enable_search(true);
	get_selection()->set_mode(SELECTION_MULTIPLE);
	m_pResultsScrolledwindow->set_flags(CAN_FOCUS);
	m_pResultsScrolledwindow->set_border_width(4);
	m_pResultsScrolledwindow->set_shadow_type(SHADOW_NONE);
	m_pResultsScrolledwindow->set_policy(POLICY_AUTOMATIC, POLICY_AUTOMATIC);
	m_pResultsScrolledwindow->property_window_placement().set_value(CORNER_TOP_LEFT);
	m_pResultsScrolledwindow->add(*this);

	// That's for the extract view
	m_extractTreeView->set_events(Gdk::BUTTON_PRESS_MASK);
	m_extractTreeView->set_flags(CAN_FOCUS);
	m_extractTreeView->set_headers_clickable(false);
	m_extractTreeView->set_headers_visible(false);
	m_extractTreeView->set_rules_hint(true);
	m_extractTreeView->set_reorderable(false);
	m_extractTreeView->set_enable_search(true);
	m_extractTreeView->get_selection()->set_mode(SELECTION_SINGLE);
	m_pExtractScrolledwindow->set_flags(CAN_FOCUS);
	m_pExtractScrolledwindow->set_border_width(4);
	m_pExtractScrolledwindow->set_shadow_type(SHADOW_NONE);
	m_pExtractScrolledwindow->set_policy(POLICY_AUTOMATIC, POLICY_AUTOMATIC);
	m_pExtractScrolledwindow->property_window_placement().set_value(CORNER_TOP_LEFT);
	m_pExtractScrolledwindow->add(*m_extractTreeView);

	// Associate the columns model to the results tree
	m_refStore = TreeStore::create(m_resultsColumns);
	set_model(m_refStore);

	if (m_groupMode != FLAT)
	{
		// The first column is for the status icons
		pColumn = new TreeViewColumn("");
		// Pack an icon renderer for the viewed status
		CellRendererPixbuf *pIconRenderer = new CellRendererPixbuf();
		pColumn->pack_start(*manage(pIconRenderer), false);
		pColumn->set_cell_data_func(*pIconRenderer, SigC::slot(*this, &ResultsTree::renderViewStatus));
		// Pack a second icon renderer for the indexed status
		pIconRenderer = new CellRendererPixbuf();
		pColumn->pack_start(*manage(pIconRenderer), false);
		pColumn->set_cell_data_func(*pIconRenderer, SigC::slot(*this, &ResultsTree::renderIndexStatus));
		// And a third one for the ranking
		pIconRenderer = new CellRendererPixbuf();
		pColumn->pack_start(*manage(pIconRenderer), false);
		pColumn->set_cell_data_func(*pIconRenderer, SigC::slot(*this, &ResultsTree::renderRanking));
		pColumn->set_sizing(TREE_VIEW_COLUMN_AUTOSIZE);
		append_column(*manage(pColumn));

		// This is the score column
		pColumn = new TreeViewColumn(_("Score"));
		CellRendererProgress *pProgressRenderer = new CellRendererProgress();
		pColumn->pack_start(*manage(pProgressRenderer));
		pColumn->add_attribute(pProgressRenderer->property_text(), m_resultsColumns.m_scoreText);
		pColumn->add_attribute(pProgressRenderer->property_value(), m_resultsColumns.m_score);
		pColumn->set_sizing(TREE_VIEW_COLUMN_AUTOSIZE);
		pColumn->set_sort_column(m_resultsColumns.m_score);
		append_column(*manage(pColumn));
	}

	// This is the title column
	pColumn = new TreeViewColumn(_("Title"));
	CellRendererText *pTextRenderer = new CellRendererText();
	pColumn->pack_start(*manage(pTextRenderer));
	pColumn->set_cell_data_func(*pTextRenderer, SigC::slot(*this, &ResultsTree::renderTitleColumn));
	pColumn->add_attribute(pTextRenderer->property_text(), m_resultsColumns.m_text);
	pColumn->set_resizable(true);
	pColumn->set_sort_column(m_resultsColumns.m_text);
	append_column(*manage(pColumn));

	// URL
	pColumn = create_column(_("URL"), m_resultsColumns.m_url, false, true, m_resultsColumns.m_url);
	if (pColumn != NULL)
	{
		pColumn->set_sizing(TREE_VIEW_COLUMN_AUTOSIZE);
		append_column(*manage(pColumn));
	}

	// The last column is for the timestamp
	pColumn = create_column(_("Date"), m_resultsColumns.m_timestamp, false, true, m_resultsColumns.m_timestampTime);
	if (pColumn != NULL)
	{
		pColumn->set_sizing(TREE_VIEW_COLUMN_AUTOSIZE);
		append_column(*manage(pColumn));
	}
	
	// Connect the signals
	signal_button_press_event().connect_notify(
		SigC::slot(*this, &ResultsTree::onButtonPressEvent));
	get_selection()->signal_changed().connect(
		SigC::slot(*this, &ResultsTree::onSelectionChanged));

	// Enable interactive search
	set_search_column(m_resultsColumns.m_text.index());
	// Control which rows can be selected
	get_selection()->set_select_function(SigC::slot(*this, &ResultsTree::onSelectionSelect));
	// Listen for style changes
	signal_style_changed().connect_notify(SigC::slot(*this, &ResultsTree::onStyleChanged));

	// Render the icons
	m_indexedIconPixbuf = render_icon(Stock::INDEX, ICON_SIZE_MENU, "MetaSE-pinot");
	m_viewededIconPixbuf = render_icon(Stock::YES, ICON_SIZE_MENU, "MetaSE-pinot");
	m_upIconPixbuf = render_icon(Stock::GO_UP, ICON_SIZE_MENU, "MetaSE-pinot");
	m_downIconPixbuf = render_icon(Stock::GO_DOWN, ICON_SIZE_MENU, "MetaSE-pinot");

	// Associate the columns model to the extract tree
	m_refExtractStore = ListStore::create(m_extractColumns);
	m_extractTreeView->set_model(m_refExtractStore);
	pColumn = new TreeViewColumn(_("Extract"));
	pTextRenderer = new CellRendererText();
	pColumn->pack_start(*manage(pTextRenderer));
	pColumn->set_cell_data_func(*pTextRenderer, SigC::slot(*this, &ResultsTree::renderExtractColumn));
	pColumn->add_attribute(pTextRenderer->property_text(), m_extractColumns.m_name);
	pColumn->set_sizing(TREE_VIEW_COLUMN_AUTOSIZE);
	m_extractTreeView->append_column(*manage(pColumn));

	// Show all
	show();
	m_pResultsScrolledwindow->show();
	m_extractTreeView->show();
	m_pExtractScrolledwindow->show();
}

ResultsTree::~ResultsTree()
{
}

void ResultsTree::renderViewStatus(CellRenderer *pRenderer, const TreeModel::iterator &iter)
{
	TreeModel::Row row = *iter;

	if (pRenderer == NULL)
	{
		return;
	}

	CellRendererPixbuf *pIconRenderer = dynamic_cast<CellRendererPixbuf*>(pRenderer);
	if (pIconRenderer != NULL)
	{
		// Has this result been already viewed ?
		if ((row[m_resultsColumns.m_viewed] == true) &&
			(m_viewededIconPixbuf))
		{
			pIconRenderer->property_pixbuf() = m_viewededIconPixbuf;
		}
		else
		{
			pIconRenderer->property_pixbuf().reset_value();
		}
	}
}

void ResultsTree::renderIndexStatus(CellRenderer *pRenderer, const TreeModel::iterator &iter)
{
	TreeModel::Row row = *iter;

	if (pRenderer == NULL)
	{
		return;
	}

	CellRendererPixbuf *pIconRenderer = dynamic_cast<CellRendererPixbuf*>(pRenderer);
	if (pIconRenderer != NULL)
	{
		// Is this result indexed ?
		if ((row[m_resultsColumns.m_indexed] == true) &&
			(m_indexedIconPixbuf))
		{
			pIconRenderer->property_pixbuf() = m_indexedIconPixbuf;
		}
		else
		{
			pIconRenderer->property_pixbuf().reset_value();
		}
	}
}

void ResultsTree::renderRanking(CellRenderer *pRenderer, const TreeModel::iterator &iter)
{
	TreeModel::Row row = *iter;

	if (pRenderer == NULL)
	{
		return;
	}

	CellRendererPixbuf *pIconRenderer = dynamic_cast<CellRendererPixbuf*>(pRenderer);
	if (pIconRenderer != NULL)
	{
		int rankDiff = row[m_resultsColumns.m_rankDiff];

		// Has this result's score changed ?
		if ((rankDiff > 0) &&
			(rankDiff != 666))
		{
			pIconRenderer->property_pixbuf() = m_upIconPixbuf;
		}
		else if (rankDiff < 0)
		{
			pIconRenderer->property_pixbuf() = m_downIconPixbuf;
		}
		else
		{
			pIconRenderer->property_pixbuf().reset_value();
		}
	}
}

void ResultsTree::renderTitleColumn(CellRenderer *pRenderer, const TreeModel::iterator &iter)
{
	TreeModel::Row row = *iter;

	if (pRenderer == NULL)
	{
		return;
	}

	CellRendererText *pTextRenderer = dynamic_cast<CellRendererText*>(pRenderer);
	if (pTextRenderer != NULL)
	{
		// Is this result new ?
		if (row[m_resultsColumns.m_rankDiff] == 666)
		{
			Color newColour;

			newColour.set_red(m_settings.m_newResultsColourRed);
			newColour.set_green(m_settings.m_newResultsColourGreen);
			newColour.set_blue(m_settings.m_newResultsColourBlue);

			// Change the row's background
			pTextRenderer->property_background_gdk() = newColour;
		}
		else
		{
			pTextRenderer->property_background_gdk().reset_value();
		}

		ResultsModelColumns::RowType type = row[m_resultsColumns.m_resultType];
		if ((type == ResultsModelColumns::ROW_ENGINE) ||
			(type == ResultsModelColumns::ROW_HOST))
		{
			ustring markup("<b>");
			markup += row[m_resultsColumns.m_text];
			markup += "</b>";
			pTextRenderer->property_markup() = markup;
		}
	}
}

void ResultsTree::renderExtractColumn(CellRenderer *pRenderer, const TreeModel::iterator &iter)
{
	TreeModel::Row row = *iter;

	if (pRenderer == NULL)
	{
		return;
	}

	CellRendererText *pTextRenderer = dynamic_cast<CellRendererText*>(pRenderer);
	if (pTextRenderer != NULL)
	{
		ustring markup(row[m_extractColumns.m_name]);
		pTextRenderer->property_markup() = markup;
		pTextRenderer->property_single_paragraph_mode() = true;
		// These properties are not available in gtkmm 2.8
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=10
		pTextRenderer->property_wrap_mode() = Pango::WRAP_WORD;
		pTextRenderer->property_wrap_width() = m_pExtractScrolledwindow->get_width();
#endif
	}
}

void ResultsTree::onButtonPressEvent(GdkEventButton *ev)
{
	// Check for popup click
	if ((ev->type == GDK_BUTTON_PRESS) &&
		(ev->button == 3) )
	{
		if (m_pPopupMenu != NULL)
		{
			m_pPopupMenu->popup(ev->button, ev->time);
		}
	}
	// Check for double clicks
	else if (ev->type == GDK_2BUTTON_PRESS)
	{
#ifdef DEBUG
		cout << "ResultsTree::onButtonPressEvent: double click on button " << ev->button << endl;
#endif
		m_signalDoubleClick();
	}
}

void ResultsTree::onSelectionChanged(void)
{
	m_signalSelectionChanged(m_treeName);
}

bool ResultsTree::onSelectionSelect(const RefPtr<TreeModel>& model,
	const TreeModel::Path& path, bool path_currently_selected)
{
	const TreeModel::iterator iter = model->get_iter(path);
	const TreeModel::Row row = *iter;

	m_indexNames.clear();

	if (path_currently_selected == false)
	{
		// Clear the extract
		m_extractTreeView->get_selection()->unselect_all();
		m_refExtractStore->clear();

		// Is this an actual result ?
		ResultsModelColumns::RowType type = row[m_resultsColumns.m_resultType];
		if (type == ResultsModelColumns::ROW_RESULT)
		{
#ifdef DEBUG
			cout << "ResultsTree::onSelectionSelect: extract for " << row[m_resultsColumns.m_text] << endl;
#endif

			TreeModel::iterator iter = m_refExtractStore->append();
			TreeModel::Row extractRow = *iter;
			extractRow[m_extractColumns.m_name] = findResultsExtract(row);
		}
	}

	return true;
}

void ResultsTree::onStyleChanged(const RefPtr<Style> &previous_style)
{
#ifdef DEBUG
	cout << "ResultsTree::onStyleChanged: called" << endl;
#endif
	// FIXME: find better icons :-)
	m_indexedIconPixbuf = render_icon(Stock::INDEX, ICON_SIZE_MENU, "MetaSE-pinot");
	m_viewededIconPixbuf = render_icon(Stock::YES, ICON_SIZE_MENU, "MetaSE-pinot");
	m_upIconPixbuf = render_icon(Stock::GO_UP, ICON_SIZE_MENU, "MetaSE-pinot");
	m_downIconPixbuf = render_icon(Stock::GO_DOWN, ICON_SIZE_MENU, "MetaSE-pinot");
}

//
// Returns the results scrolled window.
//
ScrolledWindow *ResultsTree::getResultsScrolledWindow(void) const
{
	return m_pResultsScrolledwindow;
}

//
// Returns the extract scrolled window.
//
ScrolledWindow *ResultsTree::getExtractScrolledWindow(void) const
{
	return m_pExtractScrolledwindow;
}

//
// Returns the extract tree.
//
TreeView *ResultsTree::getExtractTree(void) const
{
	return m_extractTreeView;
}

//
// Returns the extract.
//
ustring ResultsTree::getExtract(void) const
{
	ustring text;

	TreeModel::Children children = m_refExtractStore->children();
	for (TreeModel::Children::iterator iter = children.begin();
		iter != children.end(); ++iter)
	{
		TreeModel::Row row = *iter;

		text += row[m_extractColumns.m_name];
	}

	return text;
}

//
// Adds a set of results.
// Returns true if something was added to the tree.
//
bool ResultsTree::addResults(const string &engineName, const vector<DocumentInfo> &resultsList,
	const string &charset)
{
	std::map<string, TreeModel::iterator> updatedGroups;
	ResultsModelColumns::RowType rootType;
	unsigned int count = 0;

	// What's the grouping criteria ?
	if (m_groupMode == BY_ENGINE)
	{
		// By search engine
		rootType = ResultsModelColumns::ROW_ENGINE;
	}
	else if (m_groupMode == BY_HOST)
	{
		// By host
		rootType = ResultsModelColumns::ROW_HOST;
	}

	// Find out what the search engine ID is
	unsigned int indexId = 0;
	unsigned int engineId = m_settings.getEngineId(engineName);
	if (engineId == 0)
	{
		// Chances are this engine is an index
		std::map<string, string>::const_iterator mapIter = m_settings.getIndexes().find(engineName);
		if (mapIter != m_settings.getIndexes().end())
		{
			// Yes, it is
			indexId = m_settings.getIndexId(engineName);
			engineId = m_settings.getEngineId("Xapian");
#ifdef DEBUG
			cout << "ResultsTree::addResults: engine is index " << engineName << " " << indexId << endl;
#endif
		}
#ifdef DEBUG
		else cout << "ResultsTree::addResults: " << engineName << " is not an index" <<  endl;
#endif
	}
#ifdef DEBUG
	cout << "ResultsTree::addResults: ID for engine " << engineName << " is " << engineId <<  endl;
#endif

	QueryHistory queryHistory(m_settings.getHistoryDatabaseName());
	ViewHistory viewHistory(m_settings.getHistoryDatabaseName());
	bool isNewQuery = false;
	if (queryHistory.getLastRun(m_treeName, engineName).empty() == true)
	{
		isNewQuery = true;
	}

	// Look at the results list
#ifdef DEBUG
	cout << "ResultsTree::addResults: " << resultsList.size() << " results" << endl;
#endif
	for (vector<DocumentInfo>::const_iterator resultIter = resultsList.begin();
		resultIter != resultsList.end(); ++resultIter)
	{
		ustring title(to_utf8(resultIter->getTitle(), charset));
		ustring location(to_utf8(resultIter->getLocation(), charset));
		ustring extract(to_utf8(resultIter->getExtract(), charset));
		string groupName;
		TreeModel::iterator groupIter;
		float currentScore = resultIter->getScore();
		int rankDiff = 0;

		if (m_groupMode != FLAT)
		{
			// What group should the result go to ?
			if (rootType == ResultsModelColumns::ROW_HOST)
			{
				Url urlObj(location);
				groupName = urlObj.getHost();
			}
			else
			{
				groupName = engineName;
			}
			// Add the group or get its position if it's already in
			appendGroup(groupName, rootType, groupIter);

			// Has the result's ranking changed ?
			float oldestScore = 0;
			float previousScore = queryHistory.hasItem(m_treeName, engineName,
				location, oldestScore);
			if (previousScore > 0)
			{
				// Update this result whatever the current and previous rankings were
				queryHistory.updateItem(m_treeName, engineName, location,
					title, extract, charset, currentScore);
				rankDiff = (int)(currentScore - previousScore);
			}
			else
			{
				// No, this is a new result
				queryHistory.insertItem(m_treeName, engineName, location,
					resultIter->getTitle(), extract, charset, currentScore);
				// New results are displayed as such only if the query has already been run on the engine
				if (isNewQuery == false)
				{
					// This is a magic value :-)
					rankDiff = 666;
				}
			}
		}

		++count;

		unsigned int docId = resultIter->getIsIndexed(indexId);
		bool isIndexed = false;

		if (docId > 0)
		{
			isIndexed = true;
		}

		// Has it been already viewed ?
		bool wasViewed = viewHistory.hasItem(location);

		// OK, add a row for this result within the group
		TreeModel::iterator titleIter;
		if (appendResult(title, location, resultIter->getType(), (int)currentScore, rankDiff, isIndexed, wasViewed,
			docId, resultIter->getTimestamp(), engineId, indexId, titleIter, groupIter, true) == true)
		{
#ifdef DEBUG
			cout << "ResultsTree::addResults: added row for result " << count
				<< ", " << currentScore << ", " << isIndexed << " " << indexId
				<< " " << resultIter->getTimestamp() << endl;
#endif

			if (groupIter)
			{
				// Update this map, so that we know which groups need updating
				updatedGroups[groupName] = groupIter;
			}
		}
	}

	if (count > 0)
	{
#ifdef DEBUG
		cout << "ResultsTree::addResults: " << updatedGroups.size() << " groups to update" << endl;
#endif
		// Update the groups to which we have added results
		for (std::map<string, TreeModel::iterator>::iterator mapIter = updatedGroups.begin();
			mapIter != updatedGroups.end(); mapIter++)
		{
			TreeModel::iterator groupIter = mapIter->second;
			updateGroup(groupIter);
		}

		return true;
	}
	else if (m_groupMode == BY_ENGINE)
	{
		// If this didn't return any result, add an empty group
		TreeModel::iterator groupIter;
		appendGroup(engineName, rootType, groupIter);
		updateGroup(groupIter);

		return true;
	}

	return false;
}

//
// Sets how results are grouped.
//
void ResultsTree::setGroupMode(GroupByMode groupMode)
{
	ResultsModelColumns::RowType currentType, newType;

	if (m_groupMode == FLAT)
	{
		// No change possible
		return;
	}

	if (m_groupMode == groupMode)
	{
		// No change
		return;
	}
#ifdef DEBUG
	cout << "ResultsTree::setGroupMode: set to " << groupMode << endl;
#endif
	m_groupMode = groupMode;

	// Do we need to update the tree ?
	TreeModel::Children children = m_refStore->children();
	if (children.empty() == true)
	{
		return;
	}

	// What's the new grouping criteria ?
	if (m_groupMode == BY_ENGINE)
	{
		// By search engine
		currentType = ResultsModelColumns::ROW_HOST;
		newType = ResultsModelColumns::ROW_ENGINE;
	}
	else
	{
		// By host
		currentType = ResultsModelColumns::ROW_ENGINE;
		newType = ResultsModelColumns::ROW_HOST;
	}

	// Clear the map
	m_resultsGroups.clear();

	// Unselect results
	get_selection()->unselect_all();

	TreeModel::Children::iterator iter = children.begin();
	while (iter != children.end())
	{
		TreeModel::Row row = *iter;
#ifdef DEBUG
		cout << "ResultsTree::setGroupMode: looking at " << row[m_resultsColumns.m_text] << endl;
#endif
		ResultsModelColumns::RowType type = row[m_resultsColumns.m_resultType];
		// Skip new type rows
		if (type == newType)
		{
			iter++;
			continue;
		}

		TreeModel::Children groupChildren = iter->children();
		TreeModel::Children::iterator childIter = groupChildren.begin();
		// Type ROW_RESULT
		while (childIter != groupChildren.end())
		{
			TreeModel::Row childRow = *childIter;
			TreeModel::iterator groupIter, newIter;
			bool success = false;

			// We will need the URL and engines columns in all cases
			string url(from_utf8(childRow[m_resultsColumns.m_url]));
			unsigned int engineIds = childRow[m_resultsColumns.m_engines];
			unsigned int indexIds = childRow[m_resultsColumns.m_indexes];

			// Get the name of the group this should go into
			if (m_groupMode == BY_HOST)
			{
				Url urlObj(url);
#ifdef DEBUG
				cout << "ResultsTree::setGroupMode: row " << url << endl;
#endif
				// Add group
				if (appendGroup(urlObj.getHost(), newType, groupIter) == true)
				{
					// Add result
					success = appendResult(childRow[m_resultsColumns.m_text],
						childRow[m_resultsColumns.m_url],
						childRow[m_resultsColumns.m_type],
						childRow[m_resultsColumns.m_score],
						childRow[m_resultsColumns.m_rankDiff],
						childRow[m_resultsColumns.m_indexed],
						childRow[m_resultsColumns.m_viewed],
						childRow[m_resultsColumns.m_docId],
						childRow[m_resultsColumns.m_timestamp],
						engineIds, indexIds,
						newIter, groupIter, true);
				}
			}
			else
			{
				// Look at the engines column and see which engines this result is for
				set<string> engineNames;
				m_settings.getEngineNames(engineIds, engineNames);
				if (engineNames.empty() == false)
				{
#ifdef DEBUG
					cout << "ResultsTree::setGroupMode: row is for " << engineNames.size() << " engine(s)" << endl;
#endif
					// Are there indexes in the list ?
					set<string>::iterator xapianIter = engineNames.find("Xapian");
					if ((xapianIter != engineNames.end()) &&
						(indexIds > 0))
					{
						// Erase this
						engineNames.erase(xapianIter);
#ifdef DEBUG
						cout << "ResultsTree::setGroupMode: row is for index(es)" << indexIds << endl;
#endif

						// Add entries for each index name so that we can loop once on engine names
						set<string> indexNames;
						m_settings.getIndexNames(indexIds, indexNames);
						for (set<string>::iterator iter = indexNames.begin();
							iter != indexNames.end(); ++iter)
						{
							string indexName(*iter);
							engineNames.insert(indexName);
#ifdef DEBUG
							cout << "ResultsTree::setGroupMode: row is for index " << indexName << endl;
#endif
						}
					}

					for (set<string>::iterator iter = engineNames.begin();
						iter != engineNames.end(); ++iter)
					{
						string engineName(*iter);
						unsigned int indexId = 0;
						unsigned int engineId = m_settings.getEngineId(engineName);

						if (engineId == 0)
						{
							// This is actually an index, not an engine...
							indexId = m_settings.getIndexId(engineName);
							if (indexId > 0)
							{
								engineId = m_settings.getEngineId("Xapian");
							}
#ifdef DEBUG
							cout << "ResultsTree::setGroupMode: index " << indexId << endl;
#endif
						}
#ifdef DEBUG
						else cout << "ResultsTree::setGroupMode: no index" << endl;
#endif

						// Add group
						if (appendGroup(engineName, newType, groupIter) == true)
						{
							// Add result
							appendResult(childRow[m_resultsColumns.m_text],
								childRow[m_resultsColumns.m_url],
								childRow[m_resultsColumns.m_type],
								childRow[m_resultsColumns.m_score],
								childRow[m_resultsColumns.m_rankDiff],
								childRow[m_resultsColumns.m_indexed],
								childRow[m_resultsColumns.m_viewed],
								childRow[m_resultsColumns.m_docId],
								childRow[m_resultsColumns.m_timestamp],
								engineId, indexId,
								newIter, groupIter, true);
#ifdef DEBUG
							cout << "ResultsTree::setGroupMode: row for " << *iter << endl;
#endif
						}
					}

					// FIXME: make sure at least one row was added
					success = true;
				}
			}

			if (success == true)
			{
				// Delete it
				m_refStore->erase(*childIter);
				childIter = groupChildren.begin();
			}
			else
			{
				// Don't delete anything then, just go to the next child
				childIter++;
			}
		}

		// Erase this row
		m_refStore->erase(*iter);

		// Get the new first row, that way we don't have to worry about iterators validity
		iter = children.begin();
	}

	for (std::map<string, TreeModel::iterator>::iterator mapIter = m_resultsGroups.begin();
		mapIter != m_resultsGroups.end(); mapIter++)
	{
		TreeModel::iterator groupIter = mapIter->second;
		updateGroup(groupIter);
	}

	onSelectionChanged();
}

//
// Gets the first selected item's URL.
//
ustring ResultsTree::getFirstSelectionURL(void)
{
	list<TreeModel::Path> selectedItems = get_selection()->get_selected_rows();
	if (selectedItems.empty() == true)
	{
		return "";
	}

	list<TreeModel::Path>::iterator itemPath = selectedItems.begin();
	TreeModel::iterator iter = m_refStore->get_iter(*itemPath);
	TreeModel::Row row = *iter;

	return row[m_resultsColumns.m_url];
}

//
// Gets a list of selected items.
//
bool ResultsTree::getSelection(vector<DocumentInfo> &resultsList, bool skipIndexed)
{
	list<TreeModel::Path> selectedItems = get_selection()->get_selected_rows();
	if (selectedItems.empty() == true)
	{
		return false;
	}

	// Go through selected items
	for (list<TreeModel::Path>::iterator itemPath = selectedItems.begin();
		itemPath != selectedItems.end(); ++itemPath)
	{
		TreeModel::iterator iter = m_refStore->get_iter(*itemPath);
		TreeModel::Row row = *iter;

		ResultsModelColumns::RowType type = row[m_resultsColumns.m_resultType];
		if (type != ResultsModelColumns::ROW_RESULT)
		{
			continue;
		}

		bool isIndexed = row[m_resultsColumns.m_indexed];
		if ((skipIndexed == false) ||
			(isIndexed == false))
		{
			DocumentInfo current(from_utf8(row[m_resultsColumns.m_text]),
				from_utf8(row[m_resultsColumns.m_url]),
				from_utf8(row[m_resultsColumns.m_type]), "");

			if (isIndexed == true)
			{
				set<string> indexNames;
				unsigned int indexIds = row[m_resultsColumns.m_indexes];

				m_settings.getIndexNames(indexIds, indexNames);
				// Any internal index in there ?
				for (set<string>::iterator indexIter = indexNames.begin(); indexIter != indexNames.end(); ++indexIter)
				{
					if  (m_settings.isInternalIndex(*indexIter) == true)
					{
#ifdef DEBUG
						cout << "ResultsTree::getSelection: result in internal index " << *indexIter << endl;
#endif
						current.setIsIndexed(m_settings.getIndexId(*indexIter), row[m_resultsColumns.m_docId]);
						break;
					}
				}
			}

			resultsList.push_back(current);
		}
	}
#ifdef DEBUG
	cout << "ResultsTree::getSelection: " << resultsList.size() << " results selected" << endl;
#endif

	return true;
}

//
// Sets the selected items' state.
//
void ResultsTree::setSelectionState(bool viewed)
{
	list<TreeModel::Path> selectedItems = get_selection()->get_selected_rows();
	if (selectedItems.empty() == true)
	{
		return;
	}

	// Go through selected items
	for (list<TreeModel::Path>::iterator itemPath = selectedItems.begin();
		itemPath != selectedItems.end(); ++itemPath)
	{
		TreeModel::iterator iter = m_refStore->get_iter(*itemPath);
		TreeModel::Row row = *iter;
  
		ResultsModelColumns::RowType type = row[m_resultsColumns.m_resultType];
		if (type != ResultsModelColumns::ROW_RESULT)
		{
			continue;
		}

		if (viewed == true)
		{
			row[m_resultsColumns.m_viewed] = true;
		}
	}
}

//
// Updates a result's properties.
//
void ResultsTree::updateResult(const DocumentInfo &result)
{
	unsigned int indexId = 0;
	unsigned int docId = result.getIsIndexed(indexId);

	if (docId == 0)
	{
		return;
	}

	// Go through the list
	TreeModel::Children children = m_refStore->children();
	for (TreeModel::Children::iterator iter = children.begin(); iter != children.end(); ++iter)
	{
		TreeModel::Row row = *iter;

		if (docId == row[m_resultsColumns.m_docId])
		{
			updateRow(row, result.getTitle(), result.getLocation(), result.getType(),
				row[m_resultsColumns.m_score], row[m_resultsColumns.m_engines],
				row[m_resultsColumns.m_indexes], docId,
				result.getTimestamp(), ResultsModelColumns::ROW_RESULT,
				row[m_resultsColumns.m_indexed], row[m_resultsColumns.m_viewed],
				row[m_resultsColumns.m_rankDiff]);
			break;
		}
	}
}

//
// Deletes the current selection.
//
bool ResultsTree::deleteSelection(void)
{
	bool empty = false;

	// Go through selected items
	list<TreeModel::Path> selectedItems = get_selection()->get_selected_rows();
	list<TreeModel::Path>::iterator itemPath = selectedItems.begin();
	while (itemPath != selectedItems.end())
	{
		TreeModel::iterator iter = m_refStore->get_iter(*itemPath);
		TreeModel::Row row = *iter;
		TreeModel::iterator parentIter;
		bool updateParent = false;

		// This could be a group that's in the map and should be removed first
		ResultsModelColumns::RowType type = row[m_resultsColumns.m_resultType];
		if (type != ResultsModelColumns::ROW_RESULT)
		{
			string groupName(from_utf8(row[m_resultsColumns.m_text]));
			std::map<string, TreeModel::iterator>::iterator mapIter = m_resultsGroups.find(groupName);
			if (mapIter != m_resultsGroups.end())
			{
				m_resultsGroups.erase(mapIter);
#ifdef DEBUG
				cout << "ResultsTree::deleteSelection: erased group " << groupName << endl;
#endif
			}
		}
		else if (m_groupMode != FLAT)
		{
			// This item is a result
			parentIter = row.parent();
			updateParent = true;
		}

		// Unselect and erase
		get_selection()->unselect(iter);
		m_refStore->erase(row);

		// Update group ?
		if (updateParent == true)
		{
			// Update the group this result belongs to
			updateGroup(parentIter);
		}

		selectedItems = get_selection()->get_selected_rows();
		itemPath = selectedItems.begin();
	}

	TreeModel::Children children = m_refStore->children();
	empty = children.empty();

	refresh();

	return empty;
}

//
// Deletes results.
//
bool ResultsTree::deleteResults(const string &engineName)
{
	unsigned int indexId = 0;
	unsigned int engineId = m_settings.getEngineId(engineName);
	unsigned int count = 0;

	if (engineId == 0)
	{
		// Chances are this engine is an index
		std::map<string, string>::const_iterator mapIter = m_settings.getIndexes().find(engineName);
		if (mapIter != m_settings.getIndexes().end())
		{
			// Yes, it is
			indexId = m_settings.getIndexId(engineName);
			engineId = m_settings.getEngineId("Xapian");
		}
	}

	TreeModel::Children groups = m_refStore->children();
	for (TreeModel::Children::iterator parentIter = groups.begin();
		parentIter != groups.end(); ++parentIter)
	{
		TreeModel::Row row = *parentIter;

		ResultsModelColumns::RowType type = row[m_resultsColumns.m_resultType];
		if ((type != ResultsModelColumns::ROW_ENGINE) &&
			(type != ResultsModelColumns::ROW_HOST))
		{
			continue;
		}

		TreeModel::Children children = parentIter->children();
		TreeModel::Children::iterator iter = children.begin();
		while (iter != children.end())
		{
			row = *iter;

			type = row[m_resultsColumns.m_resultType];
			if ((type == ResultsModelColumns::ROW_RESULT) &&
				(row[m_resultsColumns.m_engines] == engineId) &&
				(row[m_resultsColumns.m_indexes] == indexId))
			{
				TreeModel::Children::iterator nextIter = iter;
				++nextIter;
				++count;

				// Erase this row
				m_refStore->erase(*iter);
				iter = nextIter;
				continue;
			}

			// Next
			++iter;
		}
	}

	if (count > 0)
	{
		onSelectionChanged();
#ifdef DEBUG
		cout << "ResultsTree::deleteResults: erased " << count << " rows" << endl;
#endif
		return true;
	}

	return false;
}

//
// Returns the number of rows.
//
unsigned int ResultsTree::getRowsCount(void)
{
	return m_refStore->children().size();
}

//
// Refreshes the tree.
//
void ResultsTree::refresh(void)
{
	// FIXME: not sure why, but this helps with refreshing the tree
	columns_autosize();
}

//
// Clears the tree.
//
void ResultsTree::clear(void)
{
	// Unselect results
	get_selection()->unselect_all();

	// Remove existing rows in the tree
	TreeModel::Children children = m_refStore->children();
	if (children.empty() == false)
	{
		// Clear the groups map
		m_resultsGroups.clear();

		TreeModel::Children::iterator iter = children.begin();
		while (iter != children.end())
		{
			// Erase this row
			m_refStore->erase(*iter);

			// Get the new first row
			children = m_refStore->children();
			iter = children.begin();
		}
		m_refStore->clear();

		// Clear the extract
		m_extractTreeView->get_selection()->unselect_all();
		m_refExtractStore->clear();

		onSelectionChanged();
	}
}

//
// Shows or hides the extract field.
//
void ResultsTree::showExtract(bool show)
{
	m_showExtract = show;
	if (m_showExtract == true)
	{
		// Show the extract
		m_pExtractScrolledwindow->show();
	}
	else
	{
		// Hide
		m_pExtractScrolledwindow->hide();
	}
}

//
// Exports results to a file.
//
void ResultsTree::exportResults(const string &fileName, bool csvFormat)
{
	QueryHistory queryHistory(m_settings.getHistoryDatabaseName());
	QueryProperties queryProps(m_treeName, "");
	ResultsExporter *pExporter = NULL;
	unsigned int maxResultsCount = 0;

	if (fileName.empty() == true)
	{
		return;
	}

	if (csvFormat == true)
	{
		pExporter = new CSVExporter(fileName,
			queryProps);
	}
	else
	{
		pExporter = new OpenSearchExporter(fileName,
			queryProps);
	}

	// How many results are there altogether ?
	TreeModel::Children children = m_refStore->children();
	for (TreeModel::Children::iterator iter = children.begin();
		iter != children.end(); ++iter)
	{
		TreeModel::Row row = *iter;
		ResultsModelColumns::RowType type = row[m_resultsColumns.m_resultType];

		if ((type != ResultsModelColumns::ROW_ENGINE) &&
			(type != ResultsModelColumns::ROW_HOST))
		{
			continue;
		}

		TreeModel::Children groupChildren = iter->children();
		maxResultsCount += groupChildren.size();
	}

	// Start
	pExporter->exportStart("", maxResultsCount);

	for (TreeModel::Children::iterator iter = children.begin();
		iter != children.end(); ++iter)
	{
		TreeModel::Row row = *iter;
		ResultsModelColumns::RowType type = row[m_resultsColumns.m_resultType];

		if ((type != ResultsModelColumns::ROW_ENGINE) &&
			(type != ResultsModelColumns::ROW_HOST))
		{
			continue;
		}

		TreeModel::Children groupChildren = iter->children();
		for (TreeModel::Children::iterator childIter = groupChildren.begin();
			childIter != groupChildren.end(); ++childIter)
		{
			set<string> engineNames, indexNames;
			TreeModel::Row row = *childIter;
			DocumentInfo result(from_utf8(row[m_resultsColumns.m_text]),
				from_utf8(row[m_resultsColumns.m_url]),
				from_utf8(row[m_resultsColumns.m_type]), "");
			string engineName, charset;
			unsigned int engineIds = row[m_resultsColumns.m_engines];
			unsigned int indexIds = row[m_resultsColumns.m_indexes];

#ifdef DEBUG
			cout << "ResultsTree::exportResults: engines " << engineIds << ", indexes " << indexIds << endl;
#endif
			m_settings.getEngineNames(engineIds, engineNames);
			if (engineNames.empty() == false)
			{
				// Get the first engine this result was obtained from
				engineName = *engineNames.begin();
				if (engineName == "Xapian")
				{
					m_settings.getIndexNames(indexIds, indexNames);
					if (indexNames.empty() == false)
					{
						// Use the name of the first index as engine name
						engineName = (*indexNames.begin());
					}
				}
			}
			if (m_groupMode != FLAT)
			{
				result.setExtract(queryHistory.getItemExtract(from_utf8(m_treeName),
					engineName, result.getLocation(), charset));
			}

			// Export this
			pExporter->exportResult(engineName, result);
		}
	}

	// End
	pExporter->exportEnd();

	delete pExporter;
}

//
// Returns the changed selection signal.
//
Signal1<void, ustring>& ResultsTree::getSelectionChangedSignal(void)
{
	return m_signalSelectionChanged;
}

//
// Returns the double-click signal.
//
Signal0<void>& ResultsTree::getDoubleClickSignal(void)
{
	return m_signalDoubleClick;
}

//
// Adds a new row in the results tree.
//
bool ResultsTree::appendResult(const ustring &text, const ustring &url,
	const ustring &type, int score, int rankDiff, bool isIndexed, bool wasViewed,
	unsigned int docId, const ustring &timestamp, unsigned int engineId,
	unsigned int indexId, TreeModel::iterator &newRowIter,
	const TreeModel::iterator &parentIter, bool noDuplicates)
{
	if (!parentIter)
	{
#ifdef DEBUG
		cout << "ResultsTree::appendResult: no parent" << endl;
#endif
		newRowIter = m_refStore->append();
	}
	else
	{
		const TreeModel::Row parentRow = *parentIter;

		// Merge duplicates within groups ?
		if (noDuplicates == true)
		{
			// Look for a row with the same URL and query. For instance, in group
			// by host mode, if a page is returned by several search engines, it
			// should appear only once
			TreeModel::Children children = parentRow.children();
			if (children.empty() == false)
			{
				for (TreeModel::Children::iterator childIter = children.begin();
					childIter != children.end(); ++childIter)
				{
					TreeModel::Row row = *childIter;
					if (row[m_resultsColumns.m_url] == to_utf8(url))
					{
						// Update the engines column...
						row[m_resultsColumns.m_engines] = row[m_resultsColumns.m_engines] | engineId;
						// ...and the indexes column too
						row[m_resultsColumns.m_indexes] = row[m_resultsColumns.m_indexes] | engineId;

						newRowIter = childIter;
						return true;
					}
				}
			}
		}

		newRowIter = m_refStore->append(parentRow.children());
	}

	TreeModel::Row childRow = *newRowIter;
	updateRow(childRow, text, url, type, score, engineId, indexId,
		docId, timestamp, ResultsModelColumns::ROW_RESULT, isIndexed,
		wasViewed, rankDiff);

	return true;
}

//
// Adds a results group
//
bool ResultsTree::appendGroup(const string &groupName, ResultsModelColumns::RowType groupType,
	TreeModel::iterator &groupIter)
{
	bool success = false;

	// Is this group already in ?
	std::map<string, TreeModel::iterator>::iterator mapIter = m_resultsGroups.find(groupName);
	if (mapIter == m_resultsGroups.end())
	{
		// No, it isn't: insert a new group in the tree
		groupIter = m_refStore->append();
		TreeModel::Row groupRow = *groupIter;
		updateRow(groupRow, to_utf8(groupName),
			"", "", 0, 0, 0, 0, "", groupType,
			false, false, false);

		// Update the map
		m_resultsGroups[groupName] = groupIter;
		success = true;
#ifdef DEBUG
		cout << "ResultsTree::appendGroup: updated map with " << groupName << endl;
#endif
	}
	else
	{
		// Yes, it is
		groupIter = mapIter->second;
#ifdef DEBUG
		cout << "ResultsTree::appendGroup: found " << groupName << " in map" << endl;
#endif
		success = true;
	}

	return success;
}

//
// Updates a results group.
//
void ResultsTree::updateGroup(TreeModel::iterator &groupIter)
{
	TreeModel::Row groupRow = (*groupIter);
	int averageScore = 0;

	// Check the iterator doesn't point to a result
	ResultsModelColumns::RowType type = groupRow[m_resultsColumns.m_resultType];
	if (type == ResultsModelColumns::ROW_RESULT)
	{
		return;
	}

	// Modify the "score" column to indicate the number of results in that group
	TreeModel::Children groupChildren = groupIter->children();
	if (groupChildren.empty() == false)
	{
		for (TreeModel::Children::iterator childIter = groupChildren.begin();
			childIter != groupChildren.end(); ++childIter)
		{
			TreeModel::Row row = *childIter;

			averageScore += row[m_resultsColumns.m_score];
		}

		averageScore = (int)(averageScore / groupChildren.size());
	}
	groupRow[m_resultsColumns.m_score] = averageScore;

#ifdef DEBUG
	cout << "ResultsTree::updateGroup: group " << groupRow[m_resultsColumns.m_text]
		<< " has score " << averageScore << endl;
#endif

	// Expand this group
	TreeModel::Path groupPath = m_refStore->get_path(groupIter);
	expand_row(groupPath, true);
}

//
// Updates a row.
//
void ResultsTree::updateRow(TreeModel::Row &row, const ustring &text,
	const ustring &url, const ustring &type, int score,
	unsigned int engineId, unsigned int indexId,
	unsigned int docId, const ustring &timestamp,
	ResultsModelColumns::RowType resultType, bool indexed, bool viewed, int rankDiff)
{
	try
	{
		row[m_resultsColumns.m_text] = text;
		row[m_resultsColumns.m_url] = url;
		row[m_resultsColumns.m_type] = type;
		row[m_resultsColumns.m_score] = score;
		row[m_resultsColumns.m_scoreText] = "";
		row[m_resultsColumns.m_engines] = engineId;
		row[m_resultsColumns.m_indexes] = indexId;
		row[m_resultsColumns.m_docId] = docId;
		row[m_resultsColumns.m_resultType] = resultType;
		row[m_resultsColumns.m_timestamp] = timestamp;
		row[m_resultsColumns.m_timestampTime] = TimeConverter::fromTimestamp(timestamp);

		row[m_resultsColumns.m_indexed] = indexed;
		row[m_resultsColumns.m_viewed] = viewed;
		row[m_resultsColumns.m_rankDiff] = rankDiff;
	}
	catch (Error &error)
	{
#ifdef DEBUG
		cout << "ResultsTree::updateRow: " << error.what() << endl;
#endif
	}
	catch (...)
	{
#ifdef DEBUG
		cout << "ResultsTree::updateRow: caught unknown exception" << endl;
#endif
	}
}

//
// Retrieves the extract to show for the given row.
//
ustring ResultsTree::findResultsExtract(const Gtk::TreeModel::Row &row)
{
	QueryHistory queryHistory(m_settings.getHistoryDatabaseName());
	set<string> engineNames, indexNames;
	string url(from_utf8(row[m_resultsColumns.m_url]));
	string extract, charset;
	unsigned int engineIds = row[m_resultsColumns.m_engines];
	unsigned int indexIds = row[m_resultsColumns.m_indexes];

#ifdef DEBUG
	cout << "ResultsTree::findResultsExtract: engines " << engineIds << ", indexes " << indexIds << endl;
#endif
	m_settings.getEngineNames(engineIds, engineNames);
	if (engineNames.empty() == false)
	{
		// Get the first engine this result was obtained from
		string engineName(*engineNames.begin());
		if (engineName == "Xapian")
		{
			m_settings.getIndexNames(indexIds, indexNames);
			if (indexNames.empty() == false)
			{
				// Use the name of the first index as engine name
				engineName = (*indexNames.begin());

				// Any internal index in there ?
				for (set<string>::iterator indexIter = indexNames.begin(); indexIter != indexNames.end(); ++indexIter)
				{
					if  (m_settings.isInternalIndex(*indexIter) == true)
					{
						m_indexNames.insert(*indexIter);
					}
				}
			}
		}

#ifdef DEBUG
		cout << "ResultsTree::findResultsExtract: first engine for " << url << " was " << engineName << endl;
#endif
		extract = queryHistory.getItemExtract(from_utf8(m_treeName), engineName, url, charset);
	}

	return to_utf8(extract, charset);
}

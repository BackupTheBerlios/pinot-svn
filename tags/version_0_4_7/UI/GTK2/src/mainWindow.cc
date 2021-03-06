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

#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sigc++/class_slot.h>
#include <glibmm/ustring.h>
#include <glibmm/stringutils.h>
#include <glibmm/convert.h>
#include <glibmm/thread.h>
#include <gtkmm/aboutdialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/clipboard.h>
#include <gtkmm/main.h>

#include "HtmlTokenizer.h"
#include "IndexedDocument.h"
#include "TimeConverter.h"
#include "Url.h"
#include "TokenizerFactory.h"
#include "QueryHistory.h"
#include "ViewHistory.h"
#include "DownloaderFactory.h"
#include "XapianIndex.h"
#include "SearchEngineFactory.h"
#include "XapianEngine.h"
#include "config.h"
#include "NLS.h"
#include "PinotUtils.h"
#include "mainWindow.hh"
#include "importDialog.hh"
#include "indexDialog.hh"
#include "propertiesDialog.hh"
#include "prefsDialog.hh"
#include "queryDialog.hh"

using namespace std;
using namespace Glib;
using namespace Gdk;
using namespace Gtk;

// FIXME: this ought to be configurable
unsigned int mainWindow::m_maxDocsCount = 100;
unsigned int mainWindow::m_maxThreads = 2;

mainWindow::InternalState::InternalState(mainWindow *pWindow) :
	ThreadsManager(),
	m_liveQueryLength(0),
	m_currentPage(0),
	m_browsingIndex(false),
	m_pMainWindow(pWindow)
{
	pthread_rwlock_init(&m_listsLock, NULL);
}

mainWindow::InternalState::~InternalState()
{
	pthread_rwlock_destroy(&m_listsLock);
}

bool mainWindow::InternalState::read_lock_lists(unsigned int where)
{
	if (pthread_rwlock_rdlock(&m_listsLock) == 0)
	{
#ifdef DEBUG
		cout << "mainWindow::read_lock_lists: " << where << endl;
#endif
		return true;
	}

	return false;
}

bool mainWindow::InternalState::write_lock_lists(unsigned int where)
{
	if (pthread_rwlock_wrlock(&m_listsLock) == 0)
	{
#ifdef DEBUG
		cout << "mainWindow::write_lock_lists: " << where << endl;
#endif
		return true;
	}

	return false;
}

void mainWindow::InternalState::unlock_lists(void)
{
	pthread_rwlock_unlock(&m_listsLock);
#ifdef DEBUG
	cout << "mainWindow::unlock_lists" << endl;
#endif
}

void mainWindow::InternalState::connect(void)
{
	if (m_pMainWindow != NULL)
	{
		// Connect the dispatcher
		m_threadsEndConnection = WorkerThread::getDispatcher().connect(
			SigC::slot(*m_pMainWindow, &mainWindow::on_thread_end));
#ifdef DEBUG
		cout << "mainWindow::InternalState::connect: connected" << endl;
#endif
	}
}

//
// Constructor
//
mainWindow::mainWindow() :
	m_settings(PinotSettings::getInstance()), mainWindow_glade(),
	m_pEnginesTree(NULL),
	m_pNotebook(NULL),
	m_pIndexMenu(NULL),
	m_pLabelsMenu(NULL),
	m_pHtmlView(NULL),
	m_state(this)
{
	// Reposition and resize the window
	// Make sure the coordinates and sizes make sense
	if ((m_settings.m_xPos >= 0) &&
		(m_settings.m_yPos >= 0))
	{
		move(m_settings.m_xPos, m_settings.m_yPos);
	}
	if ((m_settings.m_width > 0) &&
		(m_settings.m_height > 0))
	{
		resize(m_settings.m_width, m_settings.m_height);
	}
	if (m_settings.m_panePos >= 0)
	{
		mainHpaned->set_position(m_settings.m_panePos);
	}

	// Position the engine tree
	m_pEnginesTree = manage(new EnginesTree(enginesVbox, m_settings));
	// Connect to the "changed" signal
	m_pEnginesTree->get_selection()->signal_changed().connect(
		SigC::slot(*this, &mainWindow::on_enginesTreeviewSelection_changed));
	// Connect to the edit index signal
	m_pEnginesTree->getEditIndexSignal().connect(
		SigC::slot(*this, &mainWindow::on_editindex));

	// Enable completion on the live query field
	m_refLiveQueryList = ListStore::create(m_liveQueryColumns);
	m_refLiveQueryCompletion = EntryCompletion::create();
	m_refLiveQueryCompletion->set_model(m_refLiveQueryList);
	m_refLiveQueryCompletion->set_text_column(m_liveQueryColumns.m_name);
	m_refLiveQueryCompletion->set_minimum_key_length(3);
	m_refLiveQueryCompletion->set_popup_completion(true);
	m_refLiveQueryCompletion->set_match_func(
		SigC::slot(*this, &mainWindow::on_queryCompletion_match));
	liveQueryEntry->set_completion(m_refLiveQueryCompletion);

	// Associate the columns model to the query tree
	m_refQueryTree = ListStore::create(m_queryColumns);
	queryTreeview->set_model(m_refQueryTree);
	TreeViewColumn *treeColumn = create_resizable_column(_("Query Name"), m_queryColumns.m_name);
	if (treeColumn != NULL)
	{
		queryTreeview->append_column(*manage(treeColumn));
	}
	queryTreeview->append_column(_("Last Run"), m_queryColumns.m_lastRun);
	queryTreeview->append_column(_("Summary"), m_queryColumns.m_summary);
	// Allow only single selection
	queryTreeview->get_selection()->set_mode(SELECTION_SINGLE);
	// Connect to the "changed" signal
	queryTreeview->get_selection()->signal_changed().connect(
		SigC::slot(*this, &mainWindow::on_queryTreeviewSelection_changed));
	// Populate
	populate_queryTreeview();

	// Populate the index menu
	populate_indexMenu();

	// Create a notebook, without any page initially
	m_pNotebook = manage(new Notebook());
	m_pNotebook->set_flags(Gtk::CAN_FOCUS);
	m_pNotebook->set_show_tabs(true);
	m_pNotebook->set_show_border(true);
	m_pNotebook->set_tab_pos(Gtk::POS_TOP);
	m_pNotebook->set_scrollable(false);
	rightVbox->pack_start(*m_pNotebook, Gtk::PACK_EXPAND_WIDGET, 4);
	m_pNotebook->signal_switch_page().connect(
		SigC::slot(*this, &mainWindow::on_switch_page), false);

	// Create an HTML renderer
	m_pHtmlView = new HtmlView(NULL);
	if (m_settings.m_browseResults == false)
	{
		string startPage("file://");
		startPage += PREFIX;
		startPage += "/share/pinot/index.html";
		view_document(startPage, true);
	}

	// Gray out menu items
	editQueryButton->set_sensitive(false);
	removeQueryButton->set_sensitive(false);
	findQueryButton->set_sensitive(false);
	clearresults1->set_sensitive(false);
	showextract1->set_sensitive(false);
	groupresults1->set_sensitive(false);
	viewresults1->set_sensitive(false);
	// Hide the View Cache menu item ?
	if ((SearchEngineFactory::isSupported("googleapi") == false) ||
		(m_settings.m_googleAPIKey.empty() == true))
	{
		viewcache1->hide();
	}
	else
	{
		viewcache1->set_sensitive(false);
	}
	indexresults1->set_sensitive(false);
	viewfromindex1->set_sensitive(false);
	refreshindex1->set_sensitive(false);
	unindex1->set_sensitive(false);
	showfromindex1->set_sensitive(false);
	//viewstop1->set_sensitive(false);
	// ...and buttons
	removeIndexButton->set_sensitive(false);

	// Set tooltips
	m_tooltips.set_tip(*addIndexButton, _("Add index"));
	m_tooltips.set_tip(*removeIndexButton, _("Remove index"));

	// Connect to threads' finished signal
	m_state.connect();

	// FIXME: delete all "ignored" threads when exiting !!!
	// Fire up the mail monitor thread
	MboxHandler *pMbox = new MboxHandler();
	// Connect to its update signal
	pMbox->getUpdateSignal().connect(
		SigC::slot(*this, &mainWindow::on_message_indexupdate));
	MonitorThread *pMonitorThread = new MonitorThread(pMbox);
	start_thread(pMonitorThread, true);
	// The handler object will be deleted when the thread terminates

	// Now we are ready
	set_status(_("Ready"));
	m_pNotebook->show();
	show();
}

//
// Destructor
//
mainWindow::~mainWindow()
{
	// Save engines
	m_pEnginesTree->save();

	// Save queries
	save_queryTreeview();

	// Stop in case we were loading a page
	NotebookPageBox *pNotebookPage = get_page(_("View"), NotebookPageBox::VIEW_PAGE);
	if (pNotebookPage != NULL)
	{
		m_pHtmlView->stop();
	}
	delete m_pHtmlView;
}

//
// Load user-defined queries
//
void mainWindow::populate_queryTreeview()
{
	QueryHistory history(m_settings.m_historyDatabase);
	const std::map<string, QueryProperties> &queries = m_settings.getQueries();

	// Reset the whole tree
	queryTreeview->get_selection()->unselect_all();
	m_refQueryTree->clear();

	// Add all user-defined queries
	std::map<string, QueryProperties>::const_iterator queryIter = queries.begin();
	for (; queryIter != queries.end(); ++queryIter)
	{
		TreeModel::iterator iter = m_refQueryTree->append();
		TreeModel::Row row = *iter;
		string queryName = queryIter->first;

		row[m_queryColumns.m_name] = to_utf8(queryName);
		string lastRun = history.getLastRun(queryName);
		if (lastRun.empty() == true)
		{
			lastRun = _("N/A");
		}
		row[m_queryColumns.m_lastRun] = to_utf8(lastRun);
		string summary = queryIter->second.toString();
		if (summary.empty() == false)
		{
			row[m_queryColumns.m_summary] = to_utf8(summary);
		}
		else
		{
			row[m_queryColumns.m_summary] = _("<undefined>");
		}
		row[m_queryColumns.m_properties] = queryIter->second;
	}
}

//
// Store defined queries into the settings object
//
void mainWindow::save_queryTreeview()
{
	// Clear the current queries
	m_settings.clearQueries();

	// Go through the query tree
	TreeModel::Children children = m_refQueryTree->children();
	if (children.empty() == false)
	{
		for (TreeModel::Children::iterator iter = children.begin();
			iter != children.end(); ++iter)
		{
			TreeModel::Row row = *iter;

			// Add this query to the settings
			string name = from_utf8(row[m_queryColumns.m_name]);
			QueryProperties queryProps = row[m_queryColumns.m_properties];
#ifdef DEBUG
			cout << "mainWindow::save_queryTreeview: " << name << endl;
#endif
			m_settings.addQuery(queryProps);
		}
	}
}

//
// Populate the index menu
//
void mainWindow::populate_indexMenu()
{
	if (m_pIndexMenu == NULL)
	{
		m_pIndexMenu = new Menu();
		list1->set_submenu(*m_pIndexMenu);
	}
	else
	{
		// Clear the submenu
		m_pIndexMenu->items().clear();
	}

	SigC::Slot1<void, ustring> indexSlot = SigC::slot(*this, &mainWindow::on_index_changed);

	// Populate the submenu
	m_pIndexMenu->items().push_back(Menu_Helpers::MenuElem(_("My Documents")));
	MenuItem *pMenuItem = &m_pIndexMenu->items().back();
	// Bind the callback's parameter to the index name
	SigC::Slot0<void> documentsActivateSlot = sigc::bind(indexSlot, _("My Documents"));
	pMenuItem->signal_activate().connect(documentsActivateSlot);

	m_pIndexMenu->items().push_back(Menu_Helpers::MenuElem(_("My Email")));
	pMenuItem = &m_pIndexMenu->items().back();
	// Bind the callback's parameter to the index name
	SigC::Slot0<void> emailActivateSlot = sigc::bind(indexSlot, _("My Email"));
	pMenuItem->signal_activate().connect(emailActivateSlot);
}

//
// Query list selection changed
//
void mainWindow::on_queryTreeviewSelection_changed()
{
	TreeModel::iterator iter = queryTreeview->get_selection()->get_selected();
	// Anything selected ?
	if (iter)
	{
		// Enable those
		editQueryButton->set_sensitive(true);
		removeQueryButton->set_sensitive(true);
		findQueryButton->set_sensitive(true);
	}
	else
	{
		// Disable those
		editQueryButton->set_sensitive(false);
		removeQueryButton->set_sensitive(false);
		findQueryButton->set_sensitive(false);
	}
}

bool mainWindow::on_queryCompletion_match(const ustring &key, const TreeModel::const_iterator &iter)
{
	TreeModel::Row row = *iter;

	ustring match = row[m_liveQueryColumns.m_name];

	return true;
}

//
// Engines tree selection changed
//
void mainWindow::on_enginesTreeviewSelection_changed()
{
	list<TreeModel::Path> selectedEngines = m_pEnginesTree->getSelection();
	// If there are more than one row selected, don't bother
	if (selectedEngines.size() != 1)
	{
		return;
	}

	list<TreeModel::Path>::iterator enginePath = selectedEngines.begin();
	if (enginePath == selectedEngines.end())
	{
		return;
	}

	TreeModel::iterator engineIter = m_pEnginesTree->getIter(*enginePath);
	bool enableRemoveIndex = false;

	// Make sure it's a leaf node
	if (engineIter->children().empty() == true)
	{
		TreeModel::Row engineRow = *engineIter;

		// Is it an external index ?
		EnginesModelColumns &enginesColumns = m_pEnginesTree->getColumnRecord();
		EnginesModelColumns::EngineType engineType = engineRow[enginesColumns.m_type];
		if (engineType == EnginesModelColumns::INDEX_ENGINE)
		{
			// Yes, enable the remove index button
			enableRemoveIndex = true;
		}
	}
	removeIndexButton->set_sensitive(enableRemoveIndex);
}

//
// Results tree selection changed
//
void mainWindow::on_resultsTreeviewSelection_changed(ustring queryName)
{
	ustring url;
	bool hasSelection = false;

	NotebookPageBox *pNotebookPage = get_page(queryName, NotebookPageBox::RESULTS_PAGE);
	if (pNotebookPage != NULL)
	{
		ResultsPage *pResultsPage = dynamic_cast<ResultsPage*>(pNotebookPage);
		if (pResultsPage != NULL)
		{
			ResultsTree *pResultsTree = pResultsPage->getTree();
			if (pResultsTree != NULL)
			{
				hasSelection = pResultsTree->checkSelection();
				if (hasSelection == true)
				{
					url = pResultsTree->getFirstSelectionURL();
				}
			}
		}
	}

	if (hasSelection == true)
	{
		bool isViewable = true, isIndexable = true, isCached = false;

		Url urlObj(from_utf8(url));
		string protocol = urlObj.getProtocol();
		// FIXME: there should be a way to know which protocols can be viewed/indexed
		if (protocol == "xapian")
		{
			isViewable = isIndexable = false;
		}

		// Enable these menu items
		viewresults1->set_sensitive(isViewable);
		if (m_settings.m_browseResults == false)
		{
			if ((protocol == "http") ||
				(protocol == "https"))
			{
				isCached = true;
			}
		}
		viewcache1->set_sensitive(isCached);
		indexresults1->set_sensitive(isIndexable);

		// Show the URL in the status bar
		ustring statusText = _("Result location is");
		statusText += " ";
		statusText += url;
		set_status(statusText, true);
	}
	else
	{
		// Disable these menu items
		viewresults1->set_sensitive(false);
		viewcache1->set_sensitive(false);
		indexresults1->set_sensitive(false);

		// Reset
		set_status("");
	}
}

//
// Index tree selection changed
//
void mainWindow::on_indexTreeviewSelection_changed(ustring indexName)
{
	vector<IndexedDocument> documentsList;

	IndexPage *pIndexPage = dynamic_cast<IndexPage*>(get_page(indexName, NotebookPageBox::INDEX_PAGE));
	if (pIndexPage == NULL)
	{
		return;
	}

	IndexTree *pIndexTree = pIndexPage->getTree();
	if ((pIndexTree != NULL) &&
		(pIndexTree->getSelection(documentsList) == true))
	{
		bool isDocumentsIndex = true;
		bool canViewDocument = true;

		// Enable these menu items, unless the index is not the documents index
		if (indexName != _("My Documents"))
		{
			isDocumentsIndex = false;
		}
		if ((indexName == _("My Email")) &&
			(m_settings.m_browseResults == true))
		{
			canViewDocument = false;
		}
		viewfromindex1->set_sensitive(canViewDocument);
		refreshindex1->set_sensitive(isDocumentsIndex);
		unindex1->set_sensitive(isDocumentsIndex);
		showfromindex1->set_sensitive(true);

		// Show the URL in the status bar
		ustring statusText = _("Document location is");
		statusText += " ";
		statusText += pIndexTree->getFirstSelectionURL();
		set_status(statusText, true);
	}
	else
	{
		// No, disable these
		viewfromindex1->set_sensitive(false);
		refreshindex1->set_sensitive(false);
		unindex1->set_sensitive(false);
		showfromindex1->set_sensitive(false);
	}
}

//
// Index > List menu selected
//
void mainWindow::on_index_changed(ustring indexName)
{
	IndexTree *pIndexTree = NULL;
	IndexPage *pIndexPage = NULL;
	ustring labelName;
	bool foundPage = false;

#ifdef DEBUG
	cout << "mainWindow::on_index_changed: current index now " << indexName << endl;
#endif

	// Is there already a page for this index ?
	pIndexPage = dynamic_cast<IndexPage*>(get_page(indexName, NotebookPageBox::INDEX_PAGE));
	if (pIndexPage != NULL)
	{
		// Force a refresh
		pIndexTree = pIndexPage->getTree();
		if (pIndexTree != NULL)
		{
			pIndexTree->clear();
		}
		labelName = pIndexPage->getLabelName();
		foundPage = true;
	}

	if (foundPage == false)
	{
		NotebookTabBox *pTab = manage(new NotebookTabBox(indexName,
			NotebookPageBox::INDEX_PAGE));
		pTab->getCloseSignal().connect(
			SigC::slot(*this, &mainWindow::on_close_page));

		// Position the index tree
		pIndexTree = manage(new IndexTree(indexName, indexMenuitem->get_submenu(), m_settings));
		pIndexPage = manage(new IndexPage(indexName, pIndexTree, m_settings));
		// Connect to the "changed" signal
		pIndexTree->getSelectionChangedSignal().connect(
			SigC::slot(*this, &mainWindow::on_indexTreeviewSelection_changed));
		// Connect to the edit document signal
		pIndexTree->getEditDocumentSignal().connect(
			SigC::slot(*this, &mainWindow::on_showfromindex_activate));
		// Connect to the label changed signal
		pIndexPage->getLabelChangedSignal().connect(
			SigC::slot(*this, &mainWindow::on_label_changed));
		// ...and to the buttons clicked signals
		pIndexPage->getBackClickedSignal().connect(
			SigC::slot(*this, &mainWindow::on_indexBackButton_clicked));
		pIndexPage->getForwardClickedSignal().connect(
			SigC::slot(*this, &mainWindow::on_indexForwardButton_clicked));

		// Append the page
		if (m_state.write_lock_lists(1) == true)
		{
			int pageNum = m_pNotebook->append_page(*pIndexPage, *pTab);
			m_pNotebook->pages().back().set_tab_label_packing(false, true, Gtk::PACK_START);
			if (pageNum >= 0)
			{
				foundPage = true;
			}

			m_state.unlock_lists();
		}
	}

	if (foundPage == true)
	{
		browse_index(indexName, labelName, 0);
	}
}

//
// Index > labels combo selection changed
//
void mainWindow::on_label_changed(ustring indexName, ustring labelName)
{
	IndexPage *pIndexPage = dynamic_cast<IndexPage*>(get_page(indexName, NotebookPageBox::INDEX_PAGE));
	if (pIndexPage == NULL)
	{
		return;
	}
	IndexTree *pIndexTree = pIndexPage->getTree();
	if (pIndexTree == NULL)
	{
		return;
	}

#ifdef DEBUG
	cout << "mainWindow::on_label_changed: called on " << labelName << endl;
#endif
	browse_index(indexName, labelName, 0);
}

//
// Notebook page switch
//
void mainWindow::on_switch_page(GtkNotebookPage *p0, guint p1)
{
	// Did the page change ?
	if (m_state.m_currentPage != p1)
	{
		bool showResultsMenuitems = false;

		NotebookPageBox *pNotebookPage = dynamic_cast<NotebookPageBox*>(m_pNotebook->get_nth_page(p1));
		if (pNotebookPage != NULL)
		{
			NotebookPageBox::PageType type = pNotebookPage->getType();
			if (type == NotebookPageBox::RESULTS_PAGE)
			{
				showResultsMenuitems = true;
			}
#ifdef DEBUG
			cout << "mainWindow::on_switch_page: switched to page " << p1
				<< ", type " << type << endl;
#endif
		}

		// Results menuitems that depend on the page
		clearresults1->set_sensitive(showResultsMenuitems);
		showextract1->set_sensitive(showResultsMenuitems);
		groupresults1->set_sensitive(showResultsMenuitems);

		// Results menuitems that depend on selection
		viewresults1->set_sensitive(false);
		viewcache1->set_sensitive(false);
		indexresults1->set_sensitive(false);

		// Index menuitems that depend on selection
		viewfromindex1->set_sensitive(false);
		refreshindex1->set_sensitive(false);
		unindex1->set_sensitive(false);
		showfromindex1->set_sensitive(false);
	}
	m_state.m_currentPage = (int)p1;
}

//
// Notebook page closed
//
void mainWindow::on_close_page(ustring title, NotebookPageBox::PageType type)
{
#ifdef DEBUG
	cout << "mainWindow::on_close_page: called for tab " << title << endl;
#endif
	int pageNum = get_page_number(title, type);
	if (pageNum >= 0)
	{
		if (type == NotebookPageBox::VIEW_PAGE)
		{
			// Stop rendering
			m_pHtmlView->stop();

			// Hide, don't close the page
			Widget *pPage = m_pNotebook->get_nth_page(pageNum);
			if (pPage != NULL)
			{
				pPage->hide();
			}
		}
		else if (m_state.write_lock_lists(2) == true)
		{
			// Remove the page
			m_pNotebook->remove_page(pageNum);

			m_state.unlock_lists();
		}
	}
}

//
// End of worker thread
//
void mainWindow::on_thread_end()
{
	ustring status;

	WorkerThread *pThread = m_state.on_thread_end();
	if (pThread == NULL)
	{
#ifdef DEBUG
		cout << "mainWindow::on_thread_end: signalled but couldn't find any thread" << endl;
#endif
		return;
	}
#ifdef DEBUG
	cout << "mainWindow::on_thread_end: end of thread " << pThread->getId() << endl;
#endif

	// What type of thread was it ?
	string type = pThread->getType();
	// Did the thread fail for some reason ?
	string threadStatus = pThread->getStatus();
	if (threadStatus.empty() == false)
	{
		// Yep, it did
		set_status(to_utf8(threadStatus));
		// Better reset that flag if an error occured while browsing an index
		if (type == "IndexBrowserThread")
		{
			m_state.m_browsingIndex = false;
		}
	}
	// Based on type, take the appropriate action...
	else if (type == "IndexBrowserThread")
	{
		char docsCountStr[64];

		IndexBrowserThread *pBrowseThread = dynamic_cast<IndexBrowserThread *>(pThread);
		if (pBrowseThread == NULL)
		{
			delete pThread;
			return;
		}

		IndexPage *pIndexPage = NULL;
		IndexTree *pIndexTree = NULL;
		ustring indexName = to_utf8(pBrowseThread->getIndexName());
		ustring labelName = to_utf8(pBrowseThread->getLabelName());

		// Find the page for this index
		pIndexPage = dynamic_cast<IndexPage*>(get_page(indexName, NotebookPageBox::INDEX_PAGE));
		if (pIndexPage == NULL)
		{
			// It's probably been closed by the user
			return;
		}
		pIndexTree = pIndexPage->getTree();
		if (pIndexTree == NULL)
		{
			return;
		}

		pIndexPage->setDocumentsCount(pBrowseThread->getDocumentsCount());
		pIndexPage->updateButtonsState(m_maxDocsCount);

		status = _("Showing");
		status += " ";
		snprintf(docsCountStr, 64, "%u", pIndexTree->getRowsCount());
		status += docsCountStr;
		status += " ";
		status += _("off");
		status += " ";
		snprintf(docsCountStr, 64, "%u", pIndexPage->getDocumentsCount());
		status += docsCountStr;
		status += " ";
		status += _("documents, starting at");
		status += " ";
		snprintf(docsCountStr, 64, "%u", pIndexPage->getFirstDocument());
		status += docsCountStr;
		set_status(status);

		if (pIndexPage->getDocumentsCount() > 0)
		{
			// Refresh the tree
			pIndexTree->refresh();
		}
		m_state.m_browsingIndex = false;
	}
	else if (type == "QueryingThread")
	{
		std::map<string, TreeModel::iterator> updatedGroups;
		unsigned int count = 0;
		ResultsModelColumns::ResultType rootType;
		bool mergeDuplicates = false;
		int pageNum = -1;

		QueryingThread *pQueryThread = dynamic_cast<QueryingThread *>(pThread);
		if (pQueryThread == NULL)
		{
			delete pThread;
			return;
		}

		QueryProperties queryProps = pQueryThread->getQuery();
		ustring queryName = to_utf8(queryProps.getName());
		ustring engineName = to_utf8(pQueryThread->getEngineName());
		string resultsCharset;
		const vector<Result> &resultsList = pQueryThread->getResults(resultsCharset);

		status = _("Query");
		status += " ";
		status += queryName;
		status += " ";
		status += _("on");
		status += " ";
		status += engineName;
		status += " ";
		status += _("ended");
		set_status(status);

		// Find the page for this query
		ResultsTree *pResultsTree = NULL;
		ResultsPage *pResultsPage = dynamic_cast<ResultsPage*>(get_page(queryName, NotebookPageBox::RESULTS_PAGE));
		if (pResultsPage != NULL)
		{
			pResultsTree = pResultsPage->getTree();
			if (pResultsTree == NULL)
			{
				return;
			}
			pageNum = get_page_number(queryName, NotebookPageBox::RESULTS_PAGE);
		}
		else
		{
			// There is none, create one
			NotebookTabBox *pTab = manage(new NotebookTabBox(queryName,
				NotebookPageBox::RESULTS_PAGE));
			pTab->getCloseSignal().connect(
				SigC::slot(*this, &mainWindow::on_close_page));

			// Position the results tree
			pResultsTree = manage(new ResultsTree(queryName, resultsMenuitem->get_submenu(), m_settings));
			pResultsPage = manage(new ResultsPage(queryName, pResultsTree, m_pNotebook->get_height(), m_settings));
			// Connect to the "changed" signal
			pResultsTree->getSelectionChangedSignal().connect(
				SigC::slot(*this, &mainWindow::on_resultsTreeviewSelection_changed));

			// Append the page
			if (m_state.write_lock_lists(3) == true)
			{
				pageNum = m_pNotebook->append_page(*pResultsPage, *pTab);
				m_pNotebook->pages().back().set_tab_label_packing(false, true, Gtk::PACK_START);

				m_state.unlock_lists();
			}
		}

		if (pageNum >= 0)
		{
			// Add the results to the tree
			pResultsTree->addResults(queryProps, engineName,
				resultsList, resultsCharset,
				searchenginegroup1->get_active());
			// Switch to that page
			m_pNotebook->set_current_page(pageNum);
		}

		// Index results ?
		if ((queryProps.getIndexResults() == true) &&
			(resultsList.empty() == false))
		{
			string labelName = queryProps.getLabelName();

#ifdef DEBUG
			cout << "mainWindow::on_thread_end: indexing results, with label " << labelName << endl;
#endif
			for (vector<Result>::const_iterator resultIter = resultsList.begin();
				resultIter != resultsList.end(); ++resultIter)
			{
				// Queue this action
				queue_index(DocumentInfo(resultIter->getTitle(), resultIter->getLocation(),
					resultIter->getType(), resultIter->getLanguage()),
					labelName);
			}
		}
	}
	else if (type == "LabelUpdateThread")
	{
		LabelUpdateThread *pLabelUpdateThread = dynamic_cast<LabelUpdateThread *>(pThread);
		if (pLabelUpdateThread == NULL)
		{
			delete pThread;
			return;
		}

		status = _("Updated label(s)");
		set_status(status);
	}
	else if (type == "DownloadingThread")
	{
		DownloadingThread *pDownloadThread = dynamic_cast<DownloadingThread *>(pThread);
		if (pDownloadThread == NULL)
		{
			delete pThread;
			return;
		}

		string url = pDownloadThread->getURL();
		const Document *pDoc = pDownloadThread->getDocument();
		if (pDoc != NULL)
		{
			unsigned int dataLength = 0;

			const char *pData = pDoc->getData(dataLength);
			if ((pData != NULL) &&
				(dataLength > 0))
			{
				// Make sure settings haven't changed in the meantime
				if (m_settings.m_browseResults == false)
				{
					ustring viewName = _("View");

					// Is there still a view page ?
					ViewPage *pViewPage = dynamic_cast<ViewPage*>(get_page(viewName, NotebookPageBox::VIEW_PAGE));
					if (pViewPage != NULL)
					{
						// The page may be hidden
						pViewPage->show();
					}

					int pageNum = get_page_number(viewName, NotebookPageBox::VIEW_PAGE);
					if (pageNum >= 0)
					{
						// Display the URL in the View tab
						if (m_pHtmlView->renderData(pData, dataLength, url) == true)
						{
							//viewstop1->set_sensitive(true);
							set_status(to_utf8(url));
						}

						m_pNotebook->set_current_page(pageNum);
					}
				}
			}
		}
	}
	else if (type == "IndexingThread")
	{
		char docIdStr[64];

		IndexingThread *pIndexThread = dynamic_cast<IndexingThread *>(pThread);
		if (pIndexThread == NULL)
		{
			delete pThread;
			return;
		}

		// Get the document properties
		unsigned int docId = pIndexThread->getDocumentID();
		DocumentInfo docInfo = pIndexThread->getDocumentInfo();

		// Is the index still being shown ?
		IndexTree *pIndexTree = NULL;
		IndexPage *pIndexPage = dynamic_cast<IndexPage*>(get_page(_("My Documents"), NotebookPageBox::INDEX_PAGE));
		if (pIndexPage != NULL)
		{
			pIndexTree = pIndexPage->getTree();
		}

		// Did the thread perform an update ?
		if (pIndexThread->isNewDocument() == false)
		{
			// Yes, it did
			status = _("Updated document");
			status += " ";
			snprintf(docIdStr, 64, "%u", docId);
			status += docIdStr;

			if (pIndexTree != NULL)
			{
				// Update the index tree
				pIndexTree->updateDocumentInfo(docId, docInfo);
			}
		}
		else
		{
			string url = pIndexThread->getURL();

			status = _("Indexed");
			status += " ";
			status += to_utf8(url);

			// Update the in-progress list
			if (m_state.write_lock_lists(4) == true)
			{
				set<string>::iterator urlIter = m_state.m_beingIndexed.find(url);
				if (urlIter != m_state.m_beingIndexed.end())
				{
					m_state.m_beingIndexed.erase(urlIter);
				}

				m_state.unlock_lists();
			}

			if (pIndexTree != NULL)
			{
				unsigned int rowsCount = pIndexTree->getRowsCount();

				// Ensure the last page is being displayed and is not full
				if ((pIndexPage->getFirstDocument() + rowsCount == pIndexPage->getDocumentsCount()) &&
					(rowsCount < m_maxDocsCount))
				{
					// Add a row to the index tree
					IndexedDocument indexedDoc(docInfo.getTitle(),
						XapianEngine::buildUrl(m_settings.m_indexLocation, docId),
						docInfo.getLocation(), docInfo.getType(),
						docInfo.getLanguage());
					indexedDoc.setTimestamp(docInfo.getTimestamp());

					append_document(pIndexPage, _("My Documents"), indexedDoc);
				}
				pIndexPage->setDocumentsCount(pIndexPage->getDocumentsCount() + 1);
				pIndexPage->updateButtonsState(m_maxDocsCount);
			}
		}

		set_status(status);
	}
	else if (type == "UnindexingThread")
	{
		UnindexingThread *pUnindexThread = dynamic_cast<UnindexingThread *>(pThread);
		if (pUnindexThread == NULL)
		{
			delete pThread;
			return;
		}

		if (pUnindexThread->getDocumentsCount() > 0)
		{
			ustring indexName(_("My Documents"));
			status = _("Unindexed document(s)");
			set_status(status);

			IndexPage *pIndexPage = dynamic_cast<IndexPage*>(get_page(indexName, NotebookPageBox::INDEX_PAGE));
			if (pIndexPage != NULL)
			{
				unsigned int docsCount = pIndexPage->getDocumentsCount();

				--docsCount;
				pIndexPage->setDocumentsCount(docsCount);
				pIndexPage->updateButtonsState(m_maxDocsCount);

				if ((docsCount >= pIndexPage->getFirstDocument() + m_maxDocsCount))
				{
					// Refresh this page
					browse_index(indexName, pIndexPage->getLabelName(), pIndexPage->getFirstDocument());
				}
			}
		}
		// Else, stay silent
	}
	else if (type == "MonitorThread")
	{
		// FIXME: do something about this
	}
	else if (type == "UpdateDocumentThread")
	{
		UpdateDocumentThread *pUpdateThread = dynamic_cast<UpdateDocumentThread *>(pThread);
		if (pUpdateThread == NULL)
		{
			delete pThread;
			return;
		}

		IndexPage *pIndexPage = dynamic_cast<IndexPage*>(get_page(_("My Documents"), NotebookPageBox::INDEX_PAGE));
		if (pIndexPage != NULL)
		{
			IndexTree *pIndexTree = pIndexPage->getTree();
			if (pIndexTree != NULL)
			{
				pIndexTree->updateDocumentInfo(pUpdateThread->getDocumentID(),
					pUpdateThread->getDocumentInfo());
			}
		}

		status = _("Updated document");
		set_status(status);
	}

	// Delete the thread
	delete pThread;;

	// We might be able to run a queued action
	check_queue();

	// Any threads left to return ?
	if (m_state.get_threads_count() == 0)
	{
		if (m_timeoutConnection.connected() == true)
		{
			m_timeoutConnection.block();
			m_timeoutConnection.disconnect();
			mainProgressbar->set_fraction(0.0);
		}
	}
}

//
// Message reception by EnginesTree
//
void mainWindow::on_editindex(ustring indexName, ustring location)
{
#ifdef DEBUG
	cout << "mainWindow::on_editindex: called" << endl;
#endif
	// Open the index properties dialog
	indexDialog indexBox(indexName, location);
	indexBox.show();
	if (indexBox.run() != RESPONSE_OK)
	{
		return;
	}

	if ((indexName != indexBox.getName()) ||
		(location != indexBox.getLocation()))
	{
		ustring newName = indexBox.getName();
		ustring newLocation = indexBox.getLocation();

		// Is the name okay ?
		if (indexBox.badName() == true)
		{
			ustring statusText = _("Couldn't rename index, name");
			statusText += " ";
			statusText += newName;
			statusText += " ";
			statusText +=  _("is already in use");

			// Tell user name is bad
			set_status(statusText);
			return;
		}

		// The only way to edit an index right now is to remove it
		// first, then add it again
		if ((m_settings.removeIndex(from_utf8(indexName)) == false) ||
			(m_settings.addIndex(from_utf8(newName),
				from_utf8(newLocation)) == false))
		{
			ustring statusText = _("Couldn't rename index");
			statusText += " ";
			statusText += indexName;

			// An error occured
			set_status(statusText);
			return;
		}

		// Refresh the engines list
		m_pEnginesTree->populate();
	}

	set_status(_("Edited index"));
}

//
// Message reception from ListenerThread
//
void mainWindow::on_message_reception(DocumentInfo docInfo, string labelName)
{
	string location = docInfo.getLocation();

	if (location.empty() == false)
	{
#ifdef DEBUG
		cout << "mainWindow::on_message_reception: indexing " << location << endl;
#endif
		// Queue this
		queue_index(docInfo, labelName);
	}
}

//
// Message reception from IndexBrowserThread
//
void mainWindow::on_message_indexupdate(IndexedDocument docInfo, unsigned int docId, string indexName)
{
	// Find the page for this index
	IndexPage *pIndexPage = dynamic_cast<IndexPage*>(get_page(to_utf8(indexName), NotebookPageBox::INDEX_PAGE));
	if (pIndexPage == NULL)
	{
		// It's probably been closed by the user
#ifdef DEBUG
		cout << "mainWindow::on_message_indexupdate: " << indexName << " not being shown" << endl;
#endif
		return;
	}

	append_document(pIndexPage, to_utf8(indexName), docInfo);
}

//
// Session > Configure menu selected
//
void mainWindow::on_configure_activate()
{
	bool useExternalBrowser = m_settings.m_browseResults;

	prefsDialog prefsBox;
	prefsBox.show();
	if (prefsBox.run() != RESPONSE_OK)
	{
		return;
	}
#ifdef DEBUG
	cout << "mainWindow::on_configure_activate: settings changed" << endl;
#endif

	// FIXME: if mail accounts are configured, make sure the MonitorThread
	// is running and knows about the new accounts

	if (m_state.read_lock_lists(5) == true)
	{
		for (int pageNum = 0; pageNum < m_pNotebook->get_n_pages(); ++pageNum)
		{
			Widget *pPage = m_pNotebook->get_nth_page(pageNum);
			if (pPage != NULL)
			{
				IndexPage *pIndexPage = dynamic_cast<IndexPage*>(pPage);
				if (pIndexPage != NULL)
				{
					// Synchronize the labels list with the new settings
					pIndexPage->populateLabelCombobox();
				}
			}
		}

		m_state.unlock_lists();
	}

	// Hide the View Cache menu item ?
	if ((SearchEngineFactory::isSupported("googleapi") == false) ||
		(m_settings.m_googleAPIKey.empty() == true))
	{
		viewcache1->hide();
	}
	else
	{
		viewcache1->show();
	}

	// Do the changes affect the View tab ?
	if (useExternalBrowser != m_settings.m_browseResults)
	{
		if (m_settings.m_browseResults == true)
		{
			// Close the existing view page
			on_close_page(_("View"), NotebookPageBox::VIEW_PAGE);
		}
		else
		{
			string startPage("file://");

			// Reopen the start page
			startPage += PREFIX;
			startPage += "/share/pinot/index.html";
			view_document(startPage, true);
		}
	}

	// Any labels to delete or rename ?
	const set<string> &labelsToDelete = prefsBox.getLabelsToDelete();
	const std::map<string, string> &labelsToRename = prefsBox.getLabelsToRename();
	if ((labelsToDelete.empty() == false) ||
		(labelsToRename.empty() == false))
	{
		start_thread(new LabelUpdateThread(labelsToDelete, labelsToRename));
	}

	// Any mail documents we should delete ?
	const set<string> &mailLabelsToDelete = prefsBox.getMailLabelsToDelete();
	if (mailLabelsToDelete.empty() == false)
	{
		start_thread(new UnindexingThread(mailLabelsToDelete, from_utf8(m_settings.m_mailIndexLocation)));
	}
}

//
// Session > Quit menu selected
//
void mainWindow::on_quit_activate()
{
	on_mainWindow_delete_event(NULL);
}

//
// Edit > Cut menu selected
//
void mainWindow::on_cut_activate()
{
	// Copy
	on_copy_activate();
	// ...and delete
	on_delete_activate();
}

//
// Edit > Copy menu selected
//
void mainWindow::on_copy_activate()
{
	ustring text;

	if (liveQueryEntry->is_focus() == true)
	{
		liveQueryEntry->copy_clipboard();
		return;
	}
	else if (queryTreeview->is_focus() == true)
	{
#ifdef DEBUG
		cout << "mainWindow::on_copy_activate: query tree" << endl;
#endif
		TreeModel::iterator iter = queryTreeview->get_selection()->get_selected();
		TreeModel::Row row = *iter;
		// Copy only the query name, not the summary
		text = row[m_queryColumns.m_name];
	}
	else
	{
		// The focus may be on one of the notebook pages
		NotebookPageBox *pNotebookPage = get_current_page();
		if (pNotebookPage != NULL)
		{
			ResultsPage *pResultsPage = dynamic_cast<ResultsPage*>(pNotebookPage);
			if (pResultsPage != NULL)
			{
				vector<Result> resultsList;
				bool firstItem = true;

#ifdef DEBUG
				cout << "mainWindow::on_copy_activate: results tree" << endl;
#endif
				ResultsTree *pResultsTree = pResultsPage->getTree();
				if (pResultsTree != NULL)
				{
					// Get the current results selection
					pResultsTree->getSelection(resultsList);
				}

				for (vector<Result>::const_iterator resultIter = resultsList.begin();
					resultIter != resultsList.end(); ++resultIter)
				{
					if (firstItem == false)
					{
						text += "\n";
					}
					text += resultIter->getTitle();
					text += " ";
					text += resultIter->getLocation();
					firstItem = false;
				}
			}

			IndexPage *pIndexPage = dynamic_cast<IndexPage*>(pNotebookPage);
			if (pIndexPage != NULL)
			{
				vector<IndexedDocument> documentsList;
				bool firstItem = true;

#ifdef DEBUG
				cout << "mainWindow::on_copy_activate: index tree" << endl;
#endif
				IndexTree *pIndexTree = pIndexPage->getTree();
				if (pIndexTree != NULL)
				{
					// Get the current documents selection
					pIndexTree->getSelection(documentsList);
				}

				for (vector<IndexedDocument>::const_iterator docIter = documentsList.begin();
					docIter != documentsList.end(); ++docIter)
				{
					if (firstItem == false)
					{
						text += "\n";
					}
					text += docIter->getTitle();
					text += " ";
					text += docIter->getOriginalLocation();
					firstItem = false;
				}
			}
		}

		if (text.empty() == true)
		{
			// Only rows from the query, results and index trees can be copied
#ifdef DEBUG
			cout << "mainWindow::on_copy_activate: other" << endl;
#endif
			return;
		}
	}
	
	RefPtr<Clipboard> refClipboard = Clipboard::get();
	refClipboard->set_text(text);
}

//
// Edit > Paste menu selected
//
void mainWindow::on_paste_activate()
{
	RefPtr<Clipboard> refClipboard = Clipboard::get();
	if (refClipboard->wait_is_text_available() == false)
	{
		return;
	}

	ustring clipText = refClipboard->wait_for_text();
	if (liveQueryEntry->is_focus() == true)
	{
		int currentPosition = liveQueryEntry->get_position();

		// Paste where the cursor is
		liveQueryEntry->insert_text(clipText, clipText.length(), currentPosition);
	}
	else if (queryTreeview->is_focus() == true)
	{
#ifdef DEBUG
		cout << "mainWindow::on_paste_activate: query tree" << endl;
#endif
		// Use whatever text is in the clipboard as query name
		// FIXME: look for \n as query fields separators ?
		QueryProperties queryProps = QueryProperties(from_utf8(clipText),
			"", "", "", "");
		edit_query(queryProps, true);
	}
	else
	{
		// Only the query tree can be pasted into, others are read-only
#ifdef DEBUG
		cout << "mainWindow::on_paste_activate: other" << endl;
#endif
		return;
	}
}

//
// Edit > Delete menu selected
//
void mainWindow::on_delete_activate()
{
	if (liveQueryEntry->is_focus() == true)
	{
		liveQueryEntry->delete_selection();
	}
	else
	{
		NotebookPageBox *pNotebookPage = get_current_page();
		if (pNotebookPage != NULL)
		{
			ResultsPage *pResultsPage = dynamic_cast<ResultsPage*>(pNotebookPage);
			if (pResultsPage != NULL)
			{
#ifdef DEBUG
				cout << "mainWindow::on_delete_activate: results tree" << endl;
#endif
				ResultsTree *pResultsTree = pResultsPage->getTree();
				if (pResultsTree != NULL)
				{
					pResultsTree->deleteSelection();
				}
			}
		}
	}
	// Nothing else can be deleted
}

//
// Results > Clear menu selected
//
void mainWindow::on_clearresults_activate()
{
	NotebookPageBox *pNotebookPage = get_current_page();
	if (pNotebookPage != NULL)
	{
		ResultsPage *pResultsPage = dynamic_cast<ResultsPage*>(pNotebookPage);
		if (pResultsPage != NULL)
		{
			ResultsTree *pResultsTree = pResultsPage->getTree();
			if (pResultsTree != NULL)
			{
				pResultsTree->clear();
			}
		}
	}
}

//
// Results > Show Extract menu selected
//
void mainWindow::on_showextract_activate()
{
	NotebookPageBox *pNotebookPage = get_current_page();
	if (pNotebookPage != NULL)
	{
		ResultsPage *pResultsPage = dynamic_cast<ResultsPage*>(pNotebookPage);
		if (pResultsPage != NULL)
		{
			ResultsTree *pResultsTree = pResultsPage->getTree();
			if (pResultsTree != NULL)
			{
				pResultsTree->showExtract(showextract1->get_active());
			}
		}
	}
}

//
// Results > Group menu selected
//
void mainWindow::on_groupresults_activate()
{
	ResultsModelColumns::ResultType currentType, newType;

	// What's the new grouping criteria ?
	NotebookPageBox *pNotebookPage = get_current_page();
	if (pNotebookPage != NULL)
	{
		ResultsPage *pResultsPage = dynamic_cast<ResultsPage*>(pNotebookPage);
		if (pResultsPage != NULL)
		{
			ResultsTree *pResultsTree = pResultsPage->getTree();
			if (pResultsTree != NULL)
			{
				pResultsTree->regroupResults(searchenginegroup1->get_active());
			}
		}
	}
}

//
// Results > View menu selected
//
void mainWindow::on_viewresults_activate()
{
	NotebookPageBox *pNotebookPage = get_current_page();
	if (pNotebookPage != NULL)
	{
		ResultsPage *pResultsPage = dynamic_cast<ResultsPage*>(pNotebookPage);
		if (pResultsPage != NULL)
		{
			ResultsTree *pResultsTree = pResultsPage->getTree();
			if (pResultsTree != NULL)
			{
				ustring url = pResultsTree->getFirstSelectionURL();
				if (view_document(from_utf8(url)) == true)
				{
					// We can update the row right now
					pResultsTree->setFirstSelectionViewedState(true);
				}
			}
		}
	}
}

//
// Results > View Cache menu selected
//
void mainWindow::on_viewcache_activate()
{
	NotebookPageBox *pNotebookPage = get_current_page();
	if (pNotebookPage != NULL)
	{
		ResultsPage *pResultsPage = dynamic_cast<ResultsPage*>(pNotebookPage);
		if (pResultsPage != NULL)
		{
			ResultsTree *pResultsTree = pResultsPage->getTree();
			if (pResultsTree != NULL)
			{
				ustring url = pResultsTree->getFirstSelectionURL();

				start_thread(new DownloadingThread(url, true));

				// Update the row now, even though the cached page may not be retrieved
				pResultsTree->setFirstSelectionViewedState(true);
			}
		}
	}
}

//
// Results > Index menu selected
//
void mainWindow::on_indexresults_activate()
{
	vector<Result> resultsList;

	// Make sure this has been configured
	if (m_settings.m_indexLocation.empty() == true)
	{
		set_status(_("Please set a location for the index first"));
		return;
	}

	NotebookPageBox *pNotebookPage = get_current_page();
	if (pNotebookPage != NULL)
	{
		ResultsPage *pResultsPage = dynamic_cast<ResultsPage*>(pNotebookPage);
		if (pResultsPage != NULL)
		{
			ResultsTree *pResultsTree = pResultsPage->getTree();
			if (pResultsTree != NULL)
			{
				pResultsTree->getSelection(resultsList);
			}
		}
	}

	// Go through selected results
	for (vector<Result>::const_iterator resultIter = resultsList.begin();
		resultIter != resultsList.end(); ++resultIter)
	{
		// Get the actual URL to download
		string url = resultIter->getLocation();
	
		if (url.empty() == true)
		{
			set_status(_("Result location is unknown"));
			return;
		}
#ifdef DEBUG
		cout << "mainWindow::on_indexresults_activate: URL is " << url << endl;
#endif
		queue_index(DocumentInfo(resultIter->getTitle(), url,
			resultIter->getType(), resultIter->getLanguage()), "");
	}
}

//
// Index > Import menu selected
//
void mainWindow::on_import_activate()
{
	set<string> mimeTypes;
	int width, height;

	TokenizerFactory::getSupportedTypes(mimeTypes);
	get_size(width, height);

	m_state.disconnect();

	importDialog importBox(_("Import Document(s)"), mimeTypes);
	importBox.setHeight(height / 2);
	importBox.show();
	importBox.run();

	m_state.connect();

	// Was anything imported ?
	if (importBox.getDocumentsCount() > 0)
	{
		ustring indexName(_("My Documents"));

		// Is the index still being shown ?
		IndexPage *pIndexPage = dynamic_cast<IndexPage*>(get_page(indexName, NotebookPageBox::INDEX_PAGE));
		if (pIndexPage != NULL)
		{
			// Refresh
			browse_index(indexName, pIndexPage->getLabelName(), 0);
		}
	}
}

//
// Index > View menu selected
//
void mainWindow::on_viewfromindex_activate()
{
	IndexPage *pIndexPage = dynamic_cast<IndexPage*>(get_current_page());
	if (pIndexPage != NULL)
	{
		IndexTree *pIndexTree = pIndexPage->getTree();

		if (pIndexTree != NULL)
		{
			// View the first document, don't bother about the rest
			ustring url = pIndexTree->getFirstSelectionLiveURL();
			view_document(from_utf8(url));
		}
	}
}

//
// Index > Refresh menu selected
//
void mainWindow::on_refreshindex_activate()
{
	vector<IndexedDocument> documentsList;

	// Make sure this has been configured
	if (m_settings.m_indexLocation.empty() == true)
	{
		set_status(_("Please set a location for the index first"));
		return;
	}

	// Get the current documents selection
	IndexPage *pIndexPage = dynamic_cast<IndexPage*>(get_current_page());
	if (pIndexPage != NULL)
	{
		IndexTree *pIndexTree = pIndexPage->getTree();

		if (pIndexTree != NULL)
		{
			if ((pIndexTree->getSelection(documentsList) == false) ||
				(documentsList.empty() == true))
			{
				// No selection
				return;
			}
		}
	}

	for (vector<IndexedDocument>::const_iterator docIter = documentsList.begin();
		docIter != documentsList.end(); ++docIter)
	{
		// The document ID
		unsigned int docId = docIter->getID();
		if (docId == 0)
		{
			continue;
		}

		// The URL to download, ie the original location of the document
		string url = docIter->getOriginalLocation();
		if (url.empty() == true)
		{
			continue;
		}
#ifdef DEBUG
		cout << "mainWindow::on_refreshindex_activate: URL is " << url << endl;
#endif

		// Add this action to the queue
		DocumentInfo docInfo(docIter->getTitle(), url,
			docIter->getType(), docIter->getLanguage());
		docInfo.setTimestamp(docIter->getTimestamp());
		queue_index(docInfo, "", docId);
	}
}

//
// Index > Show Properties menu selected
//
void mainWindow::on_showfromindex_activate()
{
	vector<IndexedDocument> documentsList;
	set<string> docLabels;
	string indexName, labelName;
	DocumentInfo docInfo;
	unsigned int docId = 0;
	int width, height;
	bool matchedLabel = false, editTitle = false;

#ifdef DEBUG
	cout << "mainWindow::on_showfromindex_activate: called" << endl;
#endif
	IndexTree *pIndexTree = NULL;
	IndexPage *pIndexPage = dynamic_cast<IndexPage*>(get_current_page());
	if (pIndexPage != NULL)
	{
		indexName = from_utf8(pIndexPage->getTitle());
		labelName = from_utf8(pIndexPage->getLabelName());
		pIndexTree = pIndexPage->getTree();
	}

	const std::map<string, string> &indexesMap = PinotSettings::getInstance().getIndexes();
	std::map<string, string>::const_iterator mapIter = indexesMap.find(indexName);	
	if (mapIter == indexesMap.end())
	{
		ustring statusText = _("Index");
		statusText += " ";
		statusText += indexName;
		statusText += " ";
		statusText += _("doesn't exist");
		set_status(statusText);
		return;
	}

	XapianIndex index(mapIter->second);

	// Get the current documents selection
	if ((pIndexTree == NULL) ||
		(pIndexTree->getSelection(documentsList) == false) ||
		(documentsList.empty() == true))
	{
		// No selection
		return;
	}

	// If there's only one document selected, get its labels
	if (documentsList.size() == 1)
	{
		vector<IndexedDocument>::const_iterator docIter = documentsList.begin();

		// Get the document ID
		docId = docIter->getID();

		if (index.isGood() == true)
		{
			// Get the properties from the index because they have been altered
			// by the tree for display purposes
			index.getDocumentInfo(docId, docInfo);
			index.getDocumentLabels(docId, docLabels);

			// Does it match the current label ?
			if ((labelName.empty() == false) &&
				(find(docLabels.begin(), docLabels.end(), labelName) != docLabels.end()))
			{
				matchedLabel = true;
			}
		}
		editTitle = true;
	}
	// Else, start with a blank list

	// Let the user set the labels
	get_size(width, height);
	propertiesDialog propertiesBox(docLabels, docInfo, editTitle);
	propertiesBox.setHeight(height / 2);
	propertiesBox.show();
	if (propertiesBox.run() != RESPONSE_OK)
	{
		return;
	}
	const set<string> &labels = propertiesBox.getLabels();

	if (index.isGood() == true)
	{
		// Now apply these labels to all the documents
		for (vector<IndexedDocument>::const_iterator docIter = documentsList.begin();
			docIter != documentsList.end(); ++docIter)
		{
			// Set the document's labels list
			index.setDocumentLabels(docIter->getID(), labels);
		}
	}

	if ((documentsList.size() == 1) &&
		(docId > 0))
	{
		// Did the title or language change ?
		string newTitle = propertiesBox.getDocumentInfo().getTitle();
		string newLanguage = propertiesBox.getDocumentInfo().getLanguage();
		if ((newTitle != docInfo.getTitle()) ||
			(newLanguage != docInfo.getLanguage()))
		{
			docInfo.setTitle(newTitle);
			docInfo.setLanguage(newLanguage);
#ifdef DEBUG
			cout << "mainWindow::on_showfromindex_activate: properties changed to "
				<< newTitle << ", " << newLanguage << endl;
#endif
			
			// Update the document
			start_thread(new UpdateDocumentThread(indexName, docId, docInfo));
		}
	}
	else
	{
		if (labelName.empty() == false)
		{
			// The current label may have been applied to or removed from
			// one or more of the selected documents, so refresh the list
			start_thread(new IndexBrowserThread(indexName, labelName, 0, 0));
		}
	}
}

//
// Index > Unindex menu selected
//
void mainWindow::on_unindex_activate()
{
	vector<IndexedDocument> documentsList;
	ustring boxTitle = _("Remove this document from the index ?");

	IndexTree *pIndexTree = NULL;
	IndexPage *pIndexPage = dynamic_cast<IndexPage*>(get_current_page());
	if (pIndexPage != NULL)
	{
		pIndexTree = pIndexPage->getTree();
	}

	// Get the current documents selection
	if ((pIndexTree == NULL) ||
		(pIndexTree->getSelection(documentsList) == false) ||
		(documentsList.empty() == true))
	{
		return;
	}

	if (documentsList.size() > 1)
	{
		boxTitle = _("Remove these documents from the index ?");
	}

	// Ask for confirmation
	MessageDialog msgDialog(boxTitle, false, MESSAGE_QUESTION, BUTTONS_YES_NO);
	msgDialog.set_transient_for(*this);
	msgDialog.show();
	int result = msgDialog.run();
	if (result == RESPONSE_NO)
	{
		return;
	}

	// Remove these documents from the tree
	pIndexTree->deleteSelection();

	set<unsigned int> docIdList;
	for (vector<IndexedDocument>::const_iterator docIter = documentsList.begin();
		docIter != documentsList.end(); ++docIter)
	{
		// Get the actual URL of the document to delete
		string url = docIter->getLocation();
		string liveUrl = docIter->getOriginalLocation();
		if (url.empty() == true)
		{
			continue;
		}

		unsigned int docId = docIter->getID();
		if (docId > 0)
		{
			docIdList.insert(docId);
		}
	}

	if (docIdList.empty() == false)
	{
		// Queue this action
#ifdef DEBUG
		cout << "mainWindow::on_unindex_activate: " << docIdList.size() << " documents to unindex" << endl;
#endif
		start_thread(new UnindexingThread(docIdList));
	}
}

//
// Help > About menu selected
//
void mainWindow::on_about_activate()
{
	AboutDialog aboutBox;

	aboutBox.set_comments(_("A metasearch tool for the Free Desktop"));
	aboutBox.set_copyright(_("(C) 2005-2006 Fabrice Colin"));
	aboutBox.set_name("Pinot");
	aboutBox.set_version(VERSION);
	aboutBox.set_website("http://pinot.berlios.de/");
	aboutBox.set_website_label("http://pinot.berlios.de/");
	aboutBox.show();
	aboutBox.run();
}

//
// Activity timeout elapsed
//
bool mainWindow::on_activity_timeout()
{
	if (m_timeoutConnection.blocked() == false)
	{
		mainProgressbar->pulse();
	}
#ifdef DEBUG
	else cout << "mainWindow::on_activity_timeout: blocked" << endl;
#endif

	return true;
}

//
// Add index button click
//
void mainWindow::on_addIndexButton_clicked()
{
	// Open the index properties dialog
	indexDialog indexBox;
	indexBox.show();
	if (indexBox.run() != RESPONSE_OK)
	{
		return;
	}

	ustring name = indexBox.getName();
	ustring location = indexBox.getLocation();

	// Is the name okay ?
	if (indexBox.badName() == true)
	{
		ustring statusText = _("Index name");
		statusText += " ";
		statusText += name;
		statusText += " ";
		statusText +=  _("is already in use");

		// Tell user name is bad
		set_status(statusText);
		return;
	}

	// Add the new index
	if (m_settings.addIndex(from_utf8(name),
			from_utf8(location)) == false)
	{
		ustring statusText = _("Couldn't add index");
		statusText += " ";
		statusText += name;

		// An error occured
		set_status(statusText);
	}
	else
	{
		// Refresh the indexes list
		removeIndexButton->set_sensitive(false);
		m_pEnginesTree->populate();
	}

	set_status(_("Added new index"));
}

//
// Remove index button click
//
void mainWindow::on_removeIndexButton_clicked()
{
	list<TreeModel::Path> selectedEngines = m_pEnginesTree->getSelection();
	// If there are more than one row selected, don't bother
	if (selectedEngines.size() != 1)
	{
		return;
	}

	list<TreeModel::Path>::iterator enginePath = selectedEngines.begin();
	if (enginePath == selectedEngines.end())
	{
		return;
	}

	TreeModel::iterator engineIter = m_pEnginesTree->getIter(*enginePath);
	TreeModel::Row engineRow = *engineIter;

	// Make sure the engine is an external index
	EnginesModelColumns &engineColumns = m_pEnginesTree->getColumnRecord();
	EnginesModelColumns::EngineType engineType = engineRow[engineColumns.m_type];
	if (engineType == EnginesModelColumns::INDEX_ENGINE)
	{
		ustring name = engineRow[engineColumns.m_name];

		// Remove it
		// FIXME: ask for confirmation ?
		if (m_settings.removeIndex(from_utf8(name)) == false)
		{
			ustring statusText = _("Couldn't remove index");
			statusText += " ";
			statusText += name;

			// An error occured
			set_status(statusText);
		}
		else
		{
			// Refresh the indexes list
			removeIndexButton->set_sensitive(false);
			m_pEnginesTree->populate();
		}
	}
}

//
// Live query entry change
//
void mainWindow::on_liveQueryEntry_changed()
{
	if (m_settings.m_suggestQueryTerms == false)
	{
		// This functionality is disabled
		return;
	}

	ustring prefix, term = liveQueryEntry->get_text();
	unsigned int liveQueryLength = term.length();

	// Reset the list
	m_refLiveQueryList->clear();
	// Get the last term from the entry field
	ustring::size_type pos = term.find_last_of(" ");
	if (pos != ustring::npos)
	{
		ustring liveQuery(term);

		prefix = liveQuery.substr(0, pos);
		prefix += " ";
		term = liveQuery.substr(pos + 1);
	}

	// If characters are being deleted or if the term is too short, don't
	// attempt to offer suggestions
	if ((m_state.m_liveQueryLength > liveQueryLength) ||
		(term.empty() == true) ||
		(m_refLiveQueryCompletion->get_minimum_key_length() > term.length()))
	{
		m_state.m_liveQueryLength = liveQueryLength;
		return;
	}
	m_state.m_liveQueryLength = liveQueryLength;

	// FIXME: relying on other indices may also be useful
	XapianIndex docsIndex(m_settings.m_indexLocation);
	if (docsIndex.isGood() == true)
	{
		set<string> suggestedTerms;
		int termIndex = 0;

		// Get a list of suggestions
		docsIndex.getCloseTerms(from_utf8(term), suggestedTerms);

		// Populate the list
		for (set<string>::iterator termIter = suggestedTerms.begin();
			termIter != suggestedTerms.end(); ++termIter)
		{
			TreeModel::iterator iter = m_refLiveQueryList->append();
			TreeModel::Row row = *iter;

			row[m_liveQueryColumns.m_name] = prefix + to_utf8(*termIter);
			++termIndex;
		}
#ifdef DEBUG
		cout << "mainWindow::on_liveQueryEntry_changed: " << termIndex << " suggestions" << endl;
#endif
	}
}

//
// Return key pressed in live query entry field
//
void mainWindow::on_liveQueryEntry_activate()
{
	on_findButton_clicked();
}

//
// Find button click
//
void mainWindow::on_findButton_clicked()
{
	QueryProperties queryProps;

	queryProps.setName(_("Live query"));
	// FIXME: parse the query string !
	// Since we perform multi-step searches on the index, we might as well start with ANDing terms
	// For search plugins, this won't make a difference
	queryProps.setAndWords(from_utf8(liveQueryEntry->get_text()));

	run_search(queryProps);
}

//
// Add query button click
//
void mainWindow::on_addQueryButton_clicked()
{
	// Even though live queries terms are now ANDed together,
	// use them as OR terms when creating a new stored query
	QueryProperties queryProps = QueryProperties("", "", "", from_utf8(liveQueryEntry->get_text()), "");

	edit_query(queryProps, true);
}

//
// Edit query button click
//
void mainWindow::on_editQueryButton_clicked()
{
	TreeModel::iterator iter = queryTreeview->get_selection()->get_selected();
	// Anything selected ?
	if (iter)
	{
		TreeModel::Row row = *iter;
#ifdef DEBUG
		cout << "mainWindow::on_editQueryButton_clicked: selected " << row[m_queryColumns.m_name] << endl;
#endif

		// Edit this query's properties
		QueryProperties queryProps = row[m_queryColumns.m_properties];
		edit_query(queryProps, false);
	}
}

//
// Remove query button click
//
void mainWindow::on_removeQueryButton_clicked()
{
	TreeModel::iterator iter = queryTreeview->get_selection()->get_selected();
	// Anything selected ?
	if (iter)
	{
		TreeModel::Row row = *iter;
		string queryName = from_utf8(row[m_queryColumns.m_name]);

		if (m_settings.removeQuery(queryName) == true)
		{
			// Remove records from QueryHistory
			QueryHistory history(m_settings.m_historyDatabase);
			history.deleteItems(queryName, true);

			// Select another row
			queryTreeview->get_selection()->unselect(iter);
			TreeModel::Path path = m_refQueryTree->get_path(iter);
			path.next();
			queryTreeview->get_selection()->select(path);
			// Erase
			m_refQueryTree->erase(row);

			queryTreeview->columns_autosize();
		}
	}
}

//
// Find query button click
//
void mainWindow::on_findQueryButton_clicked()
{
	TreeModel::iterator queryIter = queryTreeview->get_selection()->get_selected();
	// Anything selected ?
	if (queryIter)
	{
		TreeModel::Row queryRow = *queryIter;

		QueryProperties queryProps = queryRow[m_queryColumns.m_properties];
		run_search(queryProps);

		// Update the Last Run column
		queryRow[m_queryColumns.m_lastRun] = to_utf8(TimeConverter::toTimestamp(time(NULL)));
	}
}

//
// Index back button click
//
void mainWindow::on_indexBackButton_clicked(ustring indexName)
{
	IndexPage *pIndexPage = dynamic_cast<IndexPage*>(get_page(indexName, NotebookPageBox::INDEX_PAGE));
	if (pIndexPage != NULL)
	{
		if (pIndexPage->getFirstDocument() >= m_maxDocsCount)
		{
			browse_index(indexName, pIndexPage->getLabelName(), pIndexPage->getFirstDocument() - m_maxDocsCount);
		}
	}
}

//
// Index forward button click
//
void mainWindow::on_indexForwardButton_clicked(ustring indexName)
{
	IndexPage *pIndexPage = dynamic_cast<IndexPage*>(get_page(indexName, NotebookPageBox::INDEX_PAGE));
	if (pIndexPage != NULL)
	{
		if (pIndexPage->getDocumentsCount() == 0)
		{
#ifdef DEBUG
			cout << "mainWindow::on_indexForwardButton_clicked: first" << endl;
#endif
			browse_index(indexName, pIndexPage->getLabelName(), 0);
		}
		else if (pIndexPage->getDocumentsCount() >= pIndexPage->getFirstDocument() + m_maxDocsCount)
		{
#ifdef DEBUG
			cout << "mainWindow::on_indexForwardButton_clicked: next" << endl;
#endif
			browse_index(indexName, pIndexPage->getLabelName(), pIndexPage->getFirstDocument() + m_maxDocsCount);
		}
#ifdef DEBUG
		cout << "mainWindow::on_indexForwardButton_clicked: counts "
			<< pIndexPage->getFirstDocument() << " " << pIndexPage->getDocumentsCount() << endl;
#endif
	}
}

//
// Query list mouse click
//
bool mainWindow::on_queryTreeview_button_press_event(GdkEventButton *ev)
{
	// Check for double clicks
	if (ev->type == GDK_2BUTTON_PRESS)
	{
		on_editQueryButton_clicked();
	}

	return false;
}

//
// Main window deleted
//
bool mainWindow::on_mainWindow_delete_event(GdkEventAny *ev)
{
	// Any thread still running ?
	if (m_state.get_threads_count() > 0)
	{
		ustring boxTitle = _("At least one task hasn't completed yet. Quit now ?");
		MessageDialog msgDialog(boxTitle, false, MESSAGE_QUESTION, BUTTONS_YES_NO);
		msgDialog.set_transient_for(*this);
		msgDialog.show();
		int result = msgDialog.run();
		if (result == RESPONSE_NO)
		{
			return true;
		}

		m_state.disconnect();
		m_state.stop_threads();
	}
	// Disconnect the threads' finished signal
	m_state.disconnect();

	// Save the window's position and dimensions now
	// Don't worry about the gravity, it hasn't been changed
	get_position(m_settings.m_xPos, m_settings.m_yPos);
	get_size(m_settings.m_width, m_settings.m_height);
	m_settings.m_panePos = mainHpaned->get_position();
#ifdef DEBUG
	cout << "mainWindow::on_mainWindow_delete_event: quitting" << endl;
#endif

	Main::quit();
	return false;
}

//
// Returns the current page.
//
NotebookPageBox *mainWindow::get_current_page(void)
{
	NotebookPageBox *pNotebookPage = NULL;

	if (m_state.read_lock_lists(6) == true)
	{
		Widget *pPage = m_pNotebook->get_nth_page(m_pNotebook->get_current_page());
		if (pPage != NULL)
		{
			pNotebookPage = dynamic_cast<NotebookPageBox*>(pPage);
		}

		m_state.unlock_lists();
	}

	return pNotebookPage;
}

//
// Returns the page with the given title.
//
NotebookPageBox *mainWindow::get_page(const ustring &title, NotebookPageBox::PageType type)
{
	NotebookPageBox *pNotebookPage = NULL;

	if (m_state.read_lock_lists(7) == true)
	{
		for (int pageNum = 0; pageNum < m_pNotebook->get_n_pages(); ++pageNum)
		{
			Widget *pPage = m_pNotebook->get_nth_page(pageNum);
			if (pPage != NULL)
			{
				pNotebookPage = dynamic_cast<NotebookPageBox*>(pPage);
				if (pNotebookPage != NULL)
				{
#ifdef DEBUG
					cout << "mainWindow::get_page: " << pNotebookPage->getTitle() << endl;
#endif
					if ((title == pNotebookPage->getTitle()) &&
						(type == pNotebookPage->getType()))
					{
						// That's the page we are looking for
						break;
					}
					pNotebookPage = NULL;
				}
			}
		}

		m_state.unlock_lists();
	}

	return pNotebookPage;
}

//
// Returns the number of the page with the given title.
//
int mainWindow::get_page_number(const ustring &title, NotebookPageBox::PageType type)
{
	int pageNumber = -1;

	if (m_state.read_lock_lists(8) == true)
	{
		for (int pageNum = 0; pageNum < m_pNotebook->get_n_pages(); ++pageNum)
		{
			Widget *pPage = m_pNotebook->get_nth_page(pageNum);
			if (pPage != NULL)
			{
				NotebookPageBox *pNotebookPage = dynamic_cast<NotebookPageBox*>(pPage);
				if (pNotebookPage != NULL)
				{
#ifdef DEBUG
					cout << "mainWindow::get_page_number: " << pNotebookPage->getTitle() << endl;
#endif
					if ((title == pNotebookPage->getTitle()) &&
						(type == pNotebookPage->getType()))
					{
						// That's the page we are looking for
						pageNumber = pageNum;
						break;
					}
				}
			}
		}

		m_state.unlock_lists();
	}

	return pageNumber;
}

//
// Queues additions to the index.
//
bool mainWindow::queue_index(const DocumentInfo &docInfo,
	const string &labelName, unsigned int docId)
{
	if (m_state.get_threads_count() >= m_maxThreads)
	{
		if (m_state.write_lock_lists(9) == true)
		{
			m_state.m_indexQueue[docInfo] = labelName;

			m_state.unlock_lists();
		}

		return true;
	}

	if (docId > 0)
	{
		// Update the document
		index_document(docInfo, labelName, docId);
	}
	else
	{
		// Index the document
		index_document(docInfo, labelName);
	}

	return false;
}

//
// Edits a query
//
void mainWindow::edit_query(QueryProperties &queryProps, bool newQuery)
{
	string queryName;

	if (newQuery == false)
	{
		// Backup the current name
		queryName = queryProps.getName();
	}

	// Edit the query's properties
	queryDialog queryBox(queryProps);
	queryBox.show();
	if (queryBox.run() != RESPONSE_OK)
	{
		// Nothing to do
		return;
	}

	// Is the name okay ?
	if (queryBox.badName() == true)
	{
		ustring statusText = _("Query name");
		statusText += " ";
		statusText += queryProps.getName();
		statusText += " ";
		statusText +=  _("is already in use");

		// Tell user the name is bad
		set_status(statusText);
		return;
	}

	if (newQuery == false)
	{
		// Did the name change ?
		string newQueryName = queryProps.getName();
		if (newQueryName != queryName)
		{
			QueryHistory history(m_settings.m_historyDatabase);

			// Remove records from QueryHistory
			history.deleteItems(queryName, true);
		}

		// Update the query properties
		if ((m_settings.removeQuery(queryName) == false) ||
			(m_settings.addQuery(queryProps) == false))
		{
			ustring statusText = _("Couldn't update query");
			statusText += " ";
			statusText += queryName;
	
			set_status(statusText);
			return;
		}

		set_status(_("Edited query"));
	}
	else
	{
		// Add the new query
		if (m_settings.addQuery(queryProps) == false)
		{
			ustring statusText = _("Couldn't add query");
			statusText += " ";
			statusText += queryProps.getName();

			set_status(statusText);
			return;
		}

		set_status(_("Added new query"));
	}

	populate_queryTreeview();
}

//
// Runs a search
//
void mainWindow::run_search(const QueryProperties &queryProps)
{
	string querySummary = queryProps.toString();
	if (querySummary.empty() == true)
	{
		set_status(_("Query is not set"));
		return;
	}
#ifdef DEBUG
	cout << "mainWindow::run_search: query name is " << queryProps.getName() << endl;
#endif

	// Check a search engine has been selected
	list<TreeModel::Path> selectedEngines = m_pEnginesTree->getSelection();
	if (selectedEngines.empty() == true)
	{
		set_status(_("No search engine selected"));
		return;
	}

	// Go through the tree and check selected nodes
	vector<TreeModel::iterator> engineIters;
	EnginesModelColumns &engineColumns = m_pEnginesTree->getColumnRecord();
	for (list<TreeModel::Path>::iterator enginePath = selectedEngines.begin();
		enginePath != selectedEngines.end(); ++enginePath)
	{
		TreeModel::iterator engineIter = m_pEnginesTree->getIter(*enginePath);
		TreeModel::Row engineRow = *engineIter;

		EnginesModelColumns::EngineType engineType = engineRow[engineColumns.m_type];
		if (engineType < EnginesModelColumns::ENGINE_FOLDER)
		{
			// Skip
			continue;
		}

		// Is it a folder ?
		if (engineType == EnginesModelColumns::ENGINE_FOLDER)
		{
			TreeModel::Children children = engineIter->children();
			for (TreeModel::Children::iterator folderEngineIter = children.begin();
				folderEngineIter != children.end(); ++folderEngineIter)
			{
				TreeModel::Row folderEngineRow = *folderEngineIter;

				EnginesModelColumns::EngineType engineType = engineRow[engineColumns.m_type];
				if (engineType < EnginesModelColumns::ENGINE_FOLDER)
				{
					// Skip
					continue;
				}

				engineIters.push_back(folderEngineIter);
			}
		}
		else
		{
			engineIters.push_back(engineIter);
		}
	}
#ifdef DEBUG
	cout << "mainWindow::run_search: selected " << engineIters.size()
		<< " engines" << endl;
#endif

	// Now go through the selected search engines
	set<ustring> engineDisplayableNames;
	for (vector<TreeModel::iterator>::iterator iter = engineIters.begin();
		iter != engineIters.end(); ++iter)
	{
		TreeModel::Row engineRow = **iter;

		// Check whether this engine has already been done
		// Using a set<TreeModel::iterator/Row> would be preferable
		// but is not helpful here
		ustring engineDisplayableName = engineRow[engineColumns.m_name];
		if (engineDisplayableNames.find(engineDisplayableName) != engineDisplayableNames.end())
		{
			continue;
		}
		engineDisplayableNames.insert(engineDisplayableName);

		ustring engineName = engineRow[engineColumns.m_engineName];
		ustring engineOption = engineRow[engineColumns.m_option];
		EnginesModelColumns::EngineType engineType = engineRow[engineColumns.m_type];
#ifdef DEBUG
		cout << "mainWindow::run_search: engine " << engineDisplayableName << endl;
#endif

		// Is it a web engine ?
		if (engineType == EnginesModelColumns::WEB_ENGINE)
		{
			// There's a special case for the Google API...
			if (engineName == "googleapi")
			{
				// Make sure this has been configured
				if (m_settings.m_googleAPIKey.empty() == true)
				{
					set_status(_("Please set the Google API key first"));
					// Skip this engine
					continue;
				}
				// Option is the Google API key
				engineOption = m_settings.m_googleAPIKey;
			}
		}

		ustring status = _("Running query");
		status += " \"";
		status += to_utf8(queryProps.getName());
		status += "\" ";
		status += _("on");
		status += " ";
		status += engineDisplayableName;
		set_status(status);

		// Spawn a new thread
		start_thread(new QueryingThread(from_utf8(engineName),
			from_utf8(engineDisplayableName), engineOption, queryProps));
	}
}

//
// Browse an index
//
void mainWindow::browse_index(const ustring &indexName,
	const ustring &labelName, unsigned int startDoc)
{
	// Rudimentary lock
	if (m_state.m_browsingIndex == true)
	{
		return;
	}
	m_state.m_browsingIndex = true;

	IndexPage *pIndexPage = dynamic_cast<IndexPage*>(get_page(indexName, NotebookPageBox::INDEX_PAGE));
	if (pIndexPage != NULL)
	{
		IndexTree *pIndexTree = pIndexPage->getTree();
		if (pIndexTree != NULL)
		{
			// Remove existing rows in the index tree
			pIndexTree->clear();
		}
		pIndexPage->setFirstDocument(startDoc);

		// Switch to that index page
		m_pNotebook->set_current_page(get_page_number(indexName, NotebookPageBox::INDEX_PAGE));
	}

	// Spawn a new thread to browse the index
	IndexBrowserThread *pBrowseThread = new IndexBrowserThread(
		from_utf8(indexName), labelName, m_maxDocsCount, startDoc);
	pBrowseThread->getUpdateSignal().connect(SigC::slot(*this,
		&mainWindow::on_message_indexupdate));
	start_thread(pBrowseThread);
}

//
// Index (or update) a document
//
void mainWindow::index_document(const DocumentInfo &docInfo,
	const string &labelName, unsigned int docId)
{
	Url urlObj(docInfo.getLocation());
	string indexLocation = m_settings.m_indexLocation;

	// If the document is mail, we need to check My Email
	if (urlObj.getProtocol() == "mailbox")
	{
		indexLocation = m_settings.m_mailIndexLocation;
	}

	XapianIndex index(indexLocation);

	// Is it an update ?
	if (docId > 0)
	{
		// Yes, it is
		start_thread(new IndexingThread(docInfo, labelName, docId));
	}
	else
	{
		string url(docInfo.getLocation());
		bool isNewDocument = false;

		// No : see if the document is already indexed
		// or is being indexed
		if (index.isGood() == true)
		{
			docId = index.hasDocument(url);
		}
		if ((docId == 0) &&
			(m_state.write_lock_lists(10) == true))
		{
			if (m_state.m_beingIndexed.find(url) == m_state.m_beingIndexed.end())
			{
				m_state.m_beingIndexed.insert(url);
				isNewDocument = true;
			}

			m_state.unlock_lists();
		}

		if (isNewDocument == true)
		{
			// This is a new document
			start_thread(new IndexingThread(docInfo, labelName));
		}
		// Complain about already indexed files only if we aren't going to set a label on them
		else if (labelName.empty() == true)
		{
			ustring status = url;
			status += " ";
			status += _("is already indexed or is being indexed");
			set_status(status);
		}
	}

	// If the document is indexed, we may have to update its labels
	if ((docId > 0) &&
		(labelName.empty() == false))
	{
		set<string> docLabels;

		// Add this new label
#ifdef DEBUG
		cout << "mainWindow::index_document: applying label " << labelName << " to document " << docId << endl;
#endif
		docLabels.insert(labelName);
		index.setDocumentLabels(docId, docLabels, false);
	}
}

//
// View a document
//
bool mainWindow::view_document(const string &url, bool internalViewerOnly)
{
	if (url.empty() == true)
	{
		set_status(_("No URL to browse"));
		return false;
	}

	// Is browsing enabled ?
	if ((internalViewerOnly == false) &&
		(m_settings.m_browseResults == true))
	{
		// Point user-defined browser to that URL
		if (m_settings.m_browserCommand.empty() == true)
		{
			set_status(_("No browser configured to view results"));
			return false;
		}

		string shellCommand = from_utf8(m_settings.m_browserCommand);
		// FIXME: do substitutions
		shellCommand += " \"";
		shellCommand += url;
		shellCommand += "\" &";
		if ((url.empty() == true) ||
			(system(shellCommand.c_str()) == -1))
		{
			ustring status = _("Couldn't browse URL:");
			status += " ";
			status += Glib::strerror(errno);
			set_status(status);
		}
	}
	else
	{
		Url urlObj(url);

		// FIXME: there should be a way to know which protocols can be viewed/indexed
		if (urlObj.getProtocol() == "mailbox")
		{
			// Get that message
			start_thread(new DownloadingThread(url, false));
		}
		else
		{
			ViewPage *pViewPage = NULL;
			ustring viewName = _("View");
			int pageNum = -1;

			// Is there already a view page ?
			pViewPage = dynamic_cast<ViewPage*>(get_page(viewName, NotebookPageBox::VIEW_PAGE));
			if (pViewPage != NULL)
			{
				pageNum = get_page_number(viewName, NotebookPageBox::VIEW_PAGE);
				// The page may be hidden
				pViewPage->show();
				// FIXME: reorder pages
			}
			else
			{
				NotebookTabBox *pTab = manage(new NotebookTabBox(viewName,
					NotebookPageBox::VIEW_PAGE));
				pTab->getCloseSignal().connect(
					SigC::slot(*this, &mainWindow::on_close_page));

				// Position everything
				pViewPage = manage(new ViewPage(viewName, m_pHtmlView, m_settings));

				// Append the page
				if (m_state.write_lock_lists(11) == true)
				{
					pageNum = m_pNotebook->append_page(*pViewPage, *pTab);
					m_pNotebook->pages().back().set_tab_label_packing(false, true, Gtk::PACK_START);

					m_state.unlock_lists();
				}
			}

			if (pageNum >= 0)
			{
				// Display the URL
				m_pNotebook->set_current_page(pageNum);
				if (m_pHtmlView->renderUrl(url) == true)
				{
					//viewstop1->set_sensitive(true);
				}
				ustring status = _("Viewing");
				status += " ";
				status += to_utf8(m_pHtmlView->getLocation());
				set_status(status);
			}
		}
	}

	// Record this into the history
	ViewHistory viewHistory(m_settings.m_historyDatabase);
	if (viewHistory.hasItem(url) == false)
	{
		viewHistory.insertItem(url);
	}

	return true;
}

//
// Append a document to the index tree.
//
bool mainWindow::append_document(IndexPage *pIndexPage, const ustring &indexName, const IndexedDocument &docInfo)
{
	bool appendToList = true;

	if (pIndexPage == NULL)
	{
		return false;
	}

	IndexTree *pIndexTree = pIndexPage->getTree();
	if (pIndexTree == NULL)
	{
		return false;
	}

	// Does that document have the current label ?
	ustring labelName = pIndexPage->getLabelName();
	if (labelName.empty() == false)
	{
		const std::map<string, string> &indexesMap = PinotSettings::getInstance().getIndexes();
		std::map<string, string>::const_iterator mapIter = indexesMap.find(indexName);
		if (mapIter != indexesMap.end())
		{
			XapianIndex index(mapIter->second);
	
			if (index.isGood() == true)
			{
				appendToList = index.hasLabel(docInfo.getID(), from_utf8(labelName));
			}
		}
	}

	if (appendToList == true)
	{
		// Add a row
		pIndexTree->appendDocument(docInfo);

		return true;
	}

	return false;
}

//
// Start of worker thread
//
bool mainWindow::start_thread(WorkerThread *pNewThread, bool inBackground)
{
	if (m_state.start_thread(pNewThread, inBackground) == false)
	{
		// Delete the object
		delete pNewThread;
		return false;
	}
#ifdef DEBUG
	cout << "mainWindow::start_thread: started thread " << pNewThread->getId() << endl;
#endif

	if (inBackground == false)
	{
		// Enable the activity progress bar
		m_timeoutConnection.block();
		m_timeoutConnection.disconnect();
		m_timeoutConnection = Glib::signal_timeout().connect(SigC::slot(*this,
			&mainWindow::on_activity_timeout), 1000);
		m_timeoutConnection.unblock();
	}

	return true;
}

//
// Checks the queue and runs the oldest action if possible.
//
bool mainWindow::check_queue(void)
{
	if (m_state.get_threads_count() >= m_maxThreads)
	{
#ifdef DEBUG
		cout << "mainWindow::check_queue: too many threads" << endl;
#endif
		return false;
	}

	DocumentInfo docInfo;
	string labelName;

	if (m_state.write_lock_lists(12) == true)
	{
		if (m_state.m_indexQueue.empty() == false)
		{
			// Get the first item
			std::map<DocumentInfo, string>::iterator queueIter = m_state.m_indexQueue.begin();
			if (queueIter != m_state.m_indexQueue.end())
			{
				docInfo = queueIter->first;
				labelName = queueIter->second;

				m_state.m_indexQueue.erase(queueIter);
			}
		}

		m_state.unlock_lists();
	}

	string location = docInfo.getLocation();
	if (location.empty() == true)
	{
		// Nothing to do
#ifdef DEBUG
		cout << "mainWindow::check_queue: empty queue" << endl;
#endif
		return false;
	}

	// Is it an update ?
	XapianIndex docsIndex(m_settings.m_indexLocation);
	unsigned int docId = docsIndex.hasDocument(location);
	if (docId > 0)
	{
		// Yes, it is
		index_document(docInfo, labelName, docId);
	}
	else
	{
		index_document(docInfo, labelName);
	}

	return true;
}

//
// Sets the status bar text.
//
void mainWindow::set_status(const ustring &text, bool canBeSkipped)
{
	static time_t lastTime = time(NULL);

	time_t now = time(NULL);
	if ((difftime(now, lastTime) < 1) &&
		(canBeSkipped == true))
	{
		// Skip this
		return;
	}
	lastTime = now;
	
	unsigned int threadsCount = m_state.get_threads_count();
	if (threadsCount > 0)
	{
		char threadsCountStr[64];

		snprintf(threadsCountStr, 64, "%u", threadsCount);
		// Display the number of threads
		mainProgressbar->set_text(threadsCountStr);
	}
	else
	{
		// Reset
		mainProgressbar->set_text("");
	}
	// Pop the previous message
	mainStatusbar->pop();
	// Push
	mainStatusbar->push(text);
}

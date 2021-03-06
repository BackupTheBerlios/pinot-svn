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

#include <iostream>
#include <gtkmm/stock.h>
#include <gtkmm/textbuffer.h>

#include "HtmlTokenizer.h"
#include "QueryHistory.h"
#include "ViewHistory.h"
#include "config.h"
#include "NLS.h"
#include "PinotSettings.h"
#include "PinotUtils.h"
#include "IndexTree.h"

using namespace std;
using namespace SigC;
using namespace Glib;
using namespace Gdk;
using namespace Gtk;

IndexTree::IndexTree(const ustring &indexName, Menu *pPopupMenu, PinotSettings &settings) :
	TreeView(),
	m_indexName(indexName),
	m_pPopupMenu(pPopupMenu),
	m_pIndexScrolledwindow(NULL),
	m_settings(settings)
{
	m_pIndexScrolledwindow = manage(new ScrolledWindow());

	// This is the actual index tree
	set_events(Gdk::BUTTON_PRESS_MASK);
	set_flags(CAN_FOCUS);
	set_headers_visible(true);
	set_rules_hint(true);
	set_reorderable(false);
	set_enable_search(true);
	m_pIndexScrolledwindow->set_flags(Gtk::CAN_FOCUS);
	m_pIndexScrolledwindow->set_border_width(4);
	m_pIndexScrolledwindow->set_shadow_type(Gtk::SHADOW_NONE);
	m_pIndexScrolledwindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_pIndexScrolledwindow->property_window_placement().set_value(Gtk::CORNER_TOP_LEFT);
	m_pIndexScrolledwindow->add(*this);

	// Associate the columns model to the index tree
	m_refStore = ListStore::create(m_indexColumns);
	set_model(m_refStore);

	// The score column is used for status icons
	TreeViewColumn *treeColumn = create_resizable_column_with_icon(_("Title"), m_indexColumns.m_text,
		SigC::slot(*this, &IndexTree::renderColour));
	if (treeColumn != NULL)
	{
		append_column(*manage(treeColumn));
	}
	treeColumn = create_resizable_column(_("URL"), m_indexColumns.m_liveUrl);
	if (treeColumn != NULL)
	{
		append_column(*manage(treeColumn));
	}
	append_column(_("Timestamp"), m_indexColumns.m_timestamp);

	// Make headers clickable
	set_headers_clickable(true);
	// Allow multiple selection
	get_selection()->set_mode(SELECTION_MULTIPLE);

	// Connect the signals
	signal_button_press_event().connect_notify(
		SigC::slot(*this, &IndexTree::onButtonPressEvent));
	get_selection()->signal_changed().connect(
		SigC::slot(*this, &IndexTree::onSelectionChanged));

	// Enable interactive search
	set_search_column(m_indexColumns.m_text.index());
	set_search_equal_func(SigC::slot(*this, &IndexTree::onSearchEqual));
	// Control which rows can be selected
	get_selection()->set_select_function(SigC::slot(*this, &IndexTree::onSelectionSelect));

	// Initially, don't display the list of indexed documents
	m_listingIndex = false;

	// By default, no particular colour is selected
	setCurrentBackgroundColour(0, 0, 0, false);

	// Show all
	m_pIndexScrolledwindow->show();
	show();
}

IndexTree::~IndexTree()
{
}

void IndexTree::renderColour(CellRenderer *renderer, const TreeModel::iterator &iter)
{
	TreeModel::Row row = *iter;

	if (renderer == NULL)
	{
		return;
	}

	CellRendererText *textRenderer = dynamic_cast<CellRendererText*>(renderer);
	if (textRenderer != NULL)
	{
		if ((m_visibleColour == false) &&
			(row[m_indexColumns.m_labeled] == true))
		{
			// Reset this
			row[m_indexColumns.m_labeled] = false;
		}
		else if (row[m_indexColumns.m_labeled] == true)
		{
			// Change the row's background
			textRenderer->property_background_gdk() = m_currentBackgroundColour;
		}
		else
		{
			textRenderer->property_background_gdk().reset_value();
		}
	}
}

void IndexTree::onButtonPressEvent(GdkEventButton *ev)
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
		cout << "IndexTree::onButtonPressEvent: double-click" << endl;
#endif
		m_signalEdit();
	}
}

void IndexTree::onSelectionChanged(void)
{
	m_signalSelectionChanged(m_indexName);
}

bool IndexTree::onSearchEqual(const RefPtr<TreeModel>& model, int column,
	const ustring& key, const TreeModel::iterator& iter)
{
}

bool IndexTree::onSelectionSelect(const RefPtr<TreeModel>& model,
		const TreeModel::Path& path, bool path_currently_selected)
{
	const TreeModel::iterator iter = model->get_iter(path);
	const TreeModel::Row row = *iter;

	if (path_currently_selected == true)
	{
#ifdef DEBUG
		cout << "IndexTree::onSelectionSelect: unselected entry " << row[m_indexColumns.m_url] << endl;
#endif
	}
	else
	{
#ifdef DEBUG
		cout << "IndexTree::onSelectionSelect: selected entry " << row[m_indexColumns.m_url] << endl;
#endif
	}

	return true;
}

//
// Returns the tree's scrolled window.
//
ScrolledWindow *IndexTree::getScrolledWindow(void) const
{
	return m_pIndexScrolledwindow;
}

//
// Appends a new row in the index tree.
//
bool IndexTree::appendDocument(const IndexedDocument &docInfo, bool withColour)
{
	TreeModel::iterator newRowIter = m_refStore->append();
	TreeModel::Row childRow = *newRowIter;
	string title = docInfo.getTitle();

	if (title.length() > 50)
	{
		string truncatedTitle = title.substr(0, 47);
		truncatedTitle += "...";
		title = truncatedTitle;
	}

	childRow[m_indexColumns.m_text] = to_utf8(title);
	childRow[m_indexColumns.m_url] = to_utf8(docInfo.getLocation());
	childRow[m_indexColumns.m_liveUrl] = to_utf8(docInfo.getOriginalLocation());
	childRow[m_indexColumns.m_type] = to_utf8(docInfo.getType());
	childRow[m_indexColumns.m_language] = to_utf8(docInfo.getLanguage());
	childRow[m_indexColumns.m_timestamp] = to_utf8(docInfo.getTimestamp());
	childRow[m_indexColumns.m_labeled] = withColour;
	childRow[m_indexColumns.m_id] = docInfo.getID();

	// If the tree was empty, it is no longer
	m_listingIndex = true;

	return true;
}


//
// Adds a set of documents.
//
bool IndexTree::addDocuments(const vector<IndexedDocument> &documentsList)
{
	unsigned int count = 0;

	// Unselect all
	get_selection()->unselect_all();

	// FIXME: clear the tree ?

	// Get the list of indexed documents
	for (vector<IndexedDocument>::const_iterator docIter = documentsList.begin();
		docIter != documentsList.end(); ++docIter)
	{
		// Add a row
		if (appendDocument(*docIter, false) == true)
		{
#ifdef DEBUG
			cout << "IndexTree::addDocuments: added row for document " << count << endl;
#endif
			count++;
		}
	}

	// Now we are listing the index contents
	m_listingIndex = true;

	return true;
}

//
// Gets the first selected item's URL.
//
ustring IndexTree::getFirstSelectionURL(void)
{
	list<TreeModel::Path> selectedItems = get_selection()->get_selected_rows();
	if (selectedItems.empty() == true)
	{
		return "";
	}

	list<TreeModel::Path>::iterator itemPath = selectedItems.begin();
	TreeModel::iterator iter = m_refStore->get_iter(*itemPath);
	TreeModel::Row row = *iter;
	return row[m_indexColumns.m_url];
}

//
// Gets the first selected item's URL.
//
ustring IndexTree::getFirstSelectionLiveURL(void)
{
	list<TreeModel::Path> selectedItems = get_selection()->get_selected_rows();
	if (selectedItems.empty() == true)
	{
		return "";
	}

	list<TreeModel::Path>::iterator itemPath = selectedItems.begin();
	TreeModel::iterator iter = m_refStore->get_iter(*itemPath);
	TreeModel::Row row = *iter;
	return row[m_indexColumns.m_liveUrl];
}

//
// Gets a list of selected items.
//
bool IndexTree::getSelection(std::vector<IndexedDocument> &documentsList)
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

		documentsList.push_back(IndexedDocument(from_utf8(row[m_indexColumns.m_text]),
			from_utf8(row[m_indexColumns.m_url]),
			from_utf8(row[m_indexColumns.m_liveUrl]),
			from_utf8(row[m_indexColumns.m_type]),
			from_utf8(row[m_indexColumns.m_language])));
	}
#ifdef DEBUG
	cout << "IndexTree::getSelection: " << documentsList.size() << " documents selected" << endl;
#endif

	return true;
}

//
// Sets the current background colour.
//
void IndexTree::setCurrentBackgroundColour(unsigned short red, unsigned short green, unsigned short blue,
	bool isVisible)
{
	m_currentBackgroundColour.set_rgb(red, green, blue);
	m_visibleColour = isVisible;
}

//
// Sets the documents that match the current background colour.
//
void IndexTree::setColourDocuments(const set<unsigned int> &documentsList)
{
	// Unselect all
	get_selection()->unselect_all();

	// Go through the list of indexed documents
	TreeModel::Children children = m_refStore->children();
	for (TreeModel::Children::iterator iter = children.begin(); iter != children.end(); ++iter)
	{
		// Does this document match the colour ?
		TreeModel::Row row = *iter;
		unsigned int docId = row[m_indexColumns.m_id];

		set<unsigned int>::iterator docIter = documentsList.find(docId);
		if (docIter != documentsList.end())
		{
			// Yes, it does !
			row[m_indexColumns.m_labeled] = true;
		}
		else
		{
			row[m_indexColumns.m_labeled] = false;
		}
	}
}

//
// Updates a document's properties.
//
void IndexTree::updateDocumentInfo(unsigned int docId, const DocumentInfo &docInfo)
{
	if (docId == 0)
	{
		return;
	}

	// Go through the list of indexed documents
	TreeModel::Children children = m_refStore->children();
	for (TreeModel::Children::iterator iter = children.begin(); iter != children.end(); ++iter)
	{
		TreeModel::Row row = *iter;

		if (docId == row[m_indexColumns.m_id])
		{
			row[m_indexColumns.m_text] = to_utf8(docInfo.getTitle());
			row[m_indexColumns.m_type] = to_utf8(docInfo.getType());
			row[m_indexColumns.m_language] = to_utf8(docInfo.getLanguage());
			row[m_indexColumns.m_timestamp] = to_utf8(docInfo.getTimestamp());
			break;
		}
	}
}

//
// Marks a document as coloured.
//
void IndexTree::setDocumentColourState(unsigned int docId, bool withColour)
{
	if (docId == 0)
	{
		return;
	}

	// Go through the list of indexed documents
	TreeModel::Children children = m_refStore->children();
	for (TreeModel::Children::iterator iter = children.begin(); iter != children.end(); ++iter)
	{
		TreeModel::Row row = *iter;

		if (docId == row[m_indexColumns.m_id])
		{
			row[m_indexColumns.m_labeled] = withColour;
			break;
		}
	}
}

//
// Deletes the current selection.
//
bool IndexTree::deleteSelection(void)
{
	bool empty = false;

	// Go through selected items
	list<TreeModel::Path> selectedItems = get_selection()->get_selected_rows();
	list<TreeModel::Path>::iterator itemPath = selectedItems.begin();
	while (itemPath != selectedItems.end())
	{
		TreeModel::iterator iter = m_refStore->get_iter(*itemPath);
		TreeModel::Row row = *iter;

		// Unselect and erase
		get_selection()->unselect(iter);
		m_refStore->erase(row);

		selectedItems = get_selection()->get_selected_rows();
		itemPath = selectedItems.begin();
	}
#ifdef DEBUG
	cout << "IndexTree::deleteSelection: deleted " << selectedItems.size() << " documents" << endl;
#endif

	TreeModel::Children children = m_refStore->children();
	if (children.empty() == true)
	{
		// The index tree is now empty
		m_listingIndex = false;
		empty = true;
	}

	return empty;
}

//
// Returns the number of rows.
//
unsigned int IndexTree::getRowsCount(void)
{
	if (m_listingIndex == false)
	{
		return 0;
	}

	// FIXME: cache this value ?
	return m_refStore->children().size();
}

//
// Refreshes the tree.
//
void IndexTree::refresh(void)
{
	// FIXME: not sure why, but this helps with refreshing the tree
	columns_autosize();
}

//
// Returns true if the tree is empty.
//
bool IndexTree::isEmpty(void)
{
	if (m_listingIndex == true)
	{
		return false;
	}

	return true;
}

//
// Clears the tree.
//
void IndexTree::clear(void)
{
	// Unselect all
	get_selection()->unselect_all();

	// Remove existing rows in the tree
	TreeModel::Children children = m_refStore->children();
	if (children.empty() == false)
	{
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

		onSelectionChanged();
	}
}

//
// Returns the edit document signal.
//
Signal0<void>& IndexTree::getEditDocumentSignal(void)
{
	return m_signalEdit;
}

//
// Returns the changed selection signal.
//
Signal1<void, ustring>& IndexTree::getSelectionChangedSignal(void)
{
	return m_signalSelectionChanged;
}

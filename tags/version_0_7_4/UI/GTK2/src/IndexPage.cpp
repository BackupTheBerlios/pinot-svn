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
#include <gtkmm/alignment.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/stock.h>
#include <gtkmm/textbuffer.h>

#include "QueryHistory.h"
#include "ViewHistory.h"
#include "config.h"
#include "NLS.h"
#include "PinotSettings.h"
#include "PinotUtils.h"
#include "IndexPage.h"

using namespace std;
using namespace SigC;
using namespace Glib;
using namespace Gtk;

IndexPage::IndexPage(const ustring &indexName, ResultsTree *pTree,
	PinotSettings &settings) :
	NotebookPageBox(indexName, NotebookPageBox::INDEX_PAGE, settings),
	m_pTree(pTree),
	m_pQueryCombobox(NULL),
	m_pBackButton(NULL),
	m_pForwardButton(NULL),
	m_docsCount(0),
	m_firstDoc(0)
{
	m_pQueryCombobox = manage(new ComboBoxText());

	Image *image521 = manage(new Image(StockID("gtk-media-rewind"), IconSize(4)));
	Label *label52 = manage(new Label(_("Show Previous")));
	HBox *hbox45 = manage(new HBox(false, 2));
	Alignment *alignment31 = manage(new Alignment(0.5, 0.5, 0, 0));
	m_pBackButton = manage(new Button());

	Image *image522 = manage(new Image(StockID("gtk-media-forward"), IconSize(4)));
	Label *label53 = manage(new Label(_("Show Next")));
	HBox *hbox46 = manage(new HBox(false, 2));
	Alignment *alignment32 = manage(new Alignment(0.5, 0.5, 0, 0));
	m_pForwardButton = manage(new Button());

	HButtonBox *indexHbuttonbox = manage(new HButtonBox(BUTTONBOX_START, 0));
	HBox *indexButtonsHbox = manage(new HBox(false, 0));

	// Buttons
	image521->set_alignment(0.5,0.5);
	image521->set_padding(0,0);
	label52->set_alignment(0.5,0.5);
	label52->set_padding(0,0);
	label52->set_justify(Gtk::JUSTIFY_LEFT);
	label52->set_line_wrap(false);
	label52->set_use_markup(false);
	label52->set_selectable(false);
	hbox45->pack_start(*image521, Gtk::PACK_SHRINK, 0);
	hbox45->pack_start(*label52, Gtk::PACK_SHRINK, 0);
	alignment31->add(*hbox45);
	m_pBackButton->set_flags(Gtk::CAN_FOCUS);
	m_pBackButton->set_flags(Gtk::CAN_DEFAULT);
	m_pBackButton->set_relief(Gtk::RELIEF_NORMAL);
	m_pBackButton->add(*alignment31);
	image522->set_alignment(0.5,0.5);
	image522->set_padding(0,0);
	label53->set_alignment(0.5,0.5);
	label53->set_padding(0,0);
	label53->set_justify(Gtk::JUSTIFY_LEFT);
	label53->set_line_wrap(false);
	label53->set_use_markup(false);
	label53->set_selectable(false);
	hbox46->pack_start(*image522, Gtk::PACK_SHRINK, 0);
	hbox46->pack_start(*label53, Gtk::PACK_SHRINK, 0);
	alignment32->add(*hbox46);
	m_pForwardButton->set_flags(Gtk::CAN_FOCUS);
	m_pForwardButton->set_flags(Gtk::CAN_DEFAULT);
	m_pForwardButton->set_relief(Gtk::RELIEF_NORMAL);
	m_pForwardButton->add(*alignment32);

	// Position everything
	indexHbuttonbox->pack_start(*m_pBackButton);
	indexHbuttonbox->pack_start(*m_pForwardButton);
	indexButtonsHbox->pack_start(*m_pQueryCombobox, Gtk::PACK_SHRINK, 4);
	indexButtonsHbox->pack_start(*indexHbuttonbox, Gtk::PACK_EXPAND_WIDGET, 4);
	pack_start(*indexButtonsHbox, Gtk::PACK_SHRINK, 4);
	if (pTree != NULL)
	{
		pack_start(*pTree->getResultsScrolledWindow());
	}

	// Populate
	populateQueryCombobox("");

	// Connect the signals
	m_pQueryCombobox->signal_changed().connect(
		SigC::slot(*this, &IndexPage::onQueryChanged));
	m_pBackButton->signal_clicked().connect(
		SigC::slot(*this, &IndexPage::onBackClicked));
	m_pForwardButton->signal_clicked().connect(
		SigC::slot(*this, &IndexPage::onForwardClicked));

	// Disable the buttons until something is being shown
	m_pBackButton->set_sensitive(false);
	m_pForwardButton->set_sensitive(false);

	// Show all
	m_pQueryCombobox->show();
	image521->show();
	label52->show();
	hbox45->show();
	alignment31->show();
	m_pBackButton->show();
	image522->show();
	label53->show();
	hbox46->show();
	alignment32->show();
	m_pForwardButton->show();
	indexHbuttonbox->show();
	indexButtonsHbox->show();
	show();
}

IndexPage::~IndexPage()
{
}

void IndexPage::onQueryChanged(void)
{
	m_queryName = m_pQueryCombobox->get_active_text();
#ifdef DEBUG
	cout << "IndexPage::onQueryChanged: current query now " << m_queryName << endl;
#endif
	if (m_queryName == _("None"))
	{
		m_queryName.clear();
	}
	m_signalQueryChanged(m_title, m_queryName);
}

void IndexPage::onBackClicked(void)
{
	m_signalBackClicked(m_title);
}

void IndexPage::onForwardClicked(void)
{
	m_signalForwardClicked(m_title);
}

//
// Returns the page's tree.
//
ResultsTree *IndexPage::getTree(void) const
{
	return m_pTree;
}

//
// Returns the name of the current query.
//
ustring IndexPage::getQueryName(void) const
{
	return m_queryName;
}

//
// Populates the queries list.
//
void IndexPage::populateQueryCombobox(const string &queryName)
{
	bool setActive = false;

	m_pQueryCombobox->clear_items();

	m_pQueryCombobox->append_text(_("None"));

	const std::map<string, QueryProperties> &queries = m_settings.getQueries();
	for (std::map<string, QueryProperties>::const_iterator queryIter = queries.begin();
		queryIter != queries.end(); ++queryIter)
	{
		m_pQueryCombobox->append_text(to_utf8(queryIter->first));

		if ((setActive == false) &&
			(queryIter->first == queryName))
		{
			m_pQueryCombobox->set_active_text(queryName);
			m_queryName = queryName;
			setActive = true;
		}
	}

	if (setActive == false)
	{
		m_pQueryCombobox->set_active(0);
		m_queryName.clear();
	}
}

//
// Updates the state of the index buttons.
//
void IndexPage::updateButtonsState(unsigned int maxDocsCount)
{
#ifdef DEBUG
	cout << "IndexPage::updateButtonsState: counts " << m_firstDoc
		<< " " << m_docsCount << endl;
#endif
	if (m_firstDoc >= maxDocsCount)
	{
		m_pBackButton->set_sensitive(true);
	}
	else
	{
		m_pBackButton->set_sensitive(false);
	}
	if (m_docsCount > m_firstDoc + maxDocsCount)
	{
		m_pForwardButton->set_sensitive(true);
	}
	else
	{
		m_pForwardButton->set_sensitive(false);
	}
}

//
// Gets the number of documents.
//
unsigned int IndexPage::getDocumentsCount(void) const
{
	return m_docsCount;
}

//
// Sets the number of documents.
//
void IndexPage::setDocumentsCount(unsigned int docsCount)
{
	m_docsCount = docsCount;
}

//
// Gets the first document.
//
unsigned int IndexPage::getFirstDocument(void) const
{
	return m_firstDoc;
}

//
// Sets the first document.
//
void IndexPage::setFirstDocument(unsigned int startDoc)
{
	m_firstDoc = startDoc;
}

//
// Returns the changed query signal.
//
Signal2<void, ustring, ustring>& IndexPage::getQueryChangedSignal(void)
{
	return m_signalQueryChanged;
}

//
// Returns the back button clicked signal.
//
Signal1<void, ustring>& IndexPage::getBackClickedSignal(void)
{
	return m_signalBackClicked;
}

//
// Returns the forward button clicked signal.
//
Signal1<void, ustring>& IndexPage::getForwardClickedSignal(void)
{
	return m_signalForwardClicked;
}

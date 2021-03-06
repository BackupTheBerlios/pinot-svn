// generated 2003/6/13 20:26:49 BST by fabrice@amra.dyndns.org.(none)
// using glademm V2.0.0
//
// newer (non customized) versions of this file go to queryDialog.cc_new

// This file is for your program, I won't touch it again!

#include <iostream>
#include <glibmm/ustring.h>
#include <glibmm/convert.h>
#include <gtkmm/menu.h>

#include "config.h"
#include "Languages.h"
#include "NLS.h"
#include "queryDialog.hh"

using namespace std;
using namespace Glib;
using namespace Gtk;

queryDialog::queryDialog(QueryProperties &properties) :
	queryDialog_glade(),
	m_name(properties.getName()),
	m_properties(properties),
	m_labels(PinotSettings::getInstance().m_labels),
	m_badName(true)
{
	string name = m_properties.getName();

	// Associate the columns model to the label combo
	m_refLabelTree = ListStore::create(m_labelColumns);
	labelCombobox->set_model(m_refLabelTree);
	labelCombobox->pack_start(m_labelColumns.m_name);
	// Associate the columns model to the language combo
	m_refLanguageTree = ListStore::create(m_languageColumns);
	languageCombobox->set_model(m_refLanguageTree);
	languageCombobox->pack_start(m_languageColumns.m_name);
	// Populate
	populate_comboboxes();

	// Name
	if (name.empty() == true)
	{
		queryOkbutton->set_sensitive(false);
	}
	else
	{
		nameEntry->set_text(locale_to_utf8(name));
	}
	// Query terms
	andEntry->set_text(locale_to_utf8(m_properties.getAndWords()));
	phraseEntry->set_text(locale_to_utf8(m_properties.getPhrase()));
	anyEntry->set_text(locale_to_utf8(m_properties.getAnyWords()));
	notEntry->set_text(locale_to_utf8(m_properties.getNotWords()));

	// Host name
	hostNameEntry->set_text(locale_to_utf8(m_properties.getHostNameFilter()));
	// File name
	fileNameEntry->set_text(locale_to_utf8(m_properties.getFileNameFilter()));
	// Maximum number of results
	resultsCountSpinbutton->set_value((double)m_properties.getMaximumResultsCount());
	// Index all results
	indexCheckbutton->set_active(m_properties.getIndexResults());
}

queryDialog::~queryDialog()
{
}

void queryDialog::populate_comboboxes()
{
	unsigned int labelNum = 1;
	bool foundLanguage = false;

	TreeModel::iterator iter = m_refLabelTree->append();
	TreeModel::Row row = *iter;
	row[m_labelColumns.m_name] = "None";
	labelCombobox->set_active(0);

#ifdef DEBUG
	cout << "queryDialog::populate_comboboxes: looking for " << m_properties.getLabelName() << endl;
#endif
	// Add all labels to the combo and select the one defined for the query
	for (set<PinotSettings::Label>::const_iterator labelIter = m_labels.begin(); labelIter != m_labels.end(); ++labelIter)
	{
		string labelName = labelIter->m_name;

		iter = m_refLabelTree->append();
		row = *iter;
		row[m_labelColumns.m_name] = locale_to_utf8(labelName);
#ifdef DEBUG
	cout << "queryDialog::populate_comboboxes: added label " << labelName << endl;
#endif

		if (labelName == m_properties.getLabelName())
		{
			labelCombobox->set_active(labelNum);
			// Keep going
		}
		++labelNum;
	}

	string queryLanguage = m_properties.getLanguage();
	iter = m_refLanguageTree->append();
	row = *iter;
	row[m_languageColumns.m_name] = _("Any");

	// Add all supported languages and select the one defined for the query
#ifdef DEBUG
	cout << "queryDialog::populate_comboboxes: looking for " << queryLanguage << endl;
#endif
	for (unsigned int languageNum = 0; languageNum < Languages::m_count; ++languageNum)
	{
		string languageName = Languages::getIntlName(languageNum);
		iter = m_refLanguageTree->append();
		row = *iter;
		row[m_languageColumns.m_name] = languageName;

		if ((foundLanguage == false) &&
			(queryLanguage.empty() == false) &&
			(queryLanguage == languageName))
		{
#ifdef DEBUG
			cout << "queryDialog::populate_comboboxes: found at " << languageNum << endl;
#endif
			languageCombobox->set_active(languageNum + 1);
			foundLanguage = true;
		}
	}
	if (foundLanguage == false)
	{
		languageCombobox->set_active(0);
	}
}

bool queryDialog::badName(void) const
{
	return m_badName;
}
void queryDialog::on_queryOkbutton_clicked()
{
	// Name
	m_properties.setName(locale_from_utf8(nameEntry->get_text()));
	m_badName = false;
	// Did the name change ?
	if (m_name != m_properties.getName())
	{
		const std::map<string, QueryProperties> &queries = PinotSettings::getInstance().getQueries();

		// Is it already used ?
		std::map<string, QueryProperties>::const_iterator queryIter = queries.find(m_properties.getName());
		if (queryIter != queries.end())
		{
			// Yes, it is
			m_badName = true;
#ifdef DEBUG
			cout << "queryDialog::on_queryOkbutton_clicked: name in use" << endl;
#endif
		}
	}

	// Query terms
	m_properties.setAndWords(locale_from_utf8(andEntry->get_text()));
	m_properties.setPhrase(locale_from_utf8(phraseEntry->get_text()));
	m_properties.setAnyWords(locale_from_utf8(anyEntry->get_text()));
	m_properties.setNotWords(locale_from_utf8(notEntry->get_text()));
	// Language
	m_properties.setLanguage("");
	int chosenLanguage = languageCombobox->get_active_row_number();
	if (chosenLanguage < Languages::m_count + 1)
	{
		if (chosenLanguage > 0)
		{
			--chosenLanguage;
			m_properties.setLanguage(Languages::getIntlName(chosenLanguage));
		}
	}
	// Host name
	m_properties.setHostNameFilter(locale_from_utf8(hostNameEntry->get_text()));
	// File name
	m_properties.setFileNameFilter(locale_from_utf8(fileNameEntry->get_text()));
	// Maximum number of results
	m_properties.setMaximumResultsCount((unsigned int)resultsCountSpinbutton->get_value());
	// Index all results
	m_properties.setIndexResults(indexCheckbutton->get_active());
	// Label
	int chosenLabel = labelCombobox->get_active_row_number();
#ifdef DEBUG
	cout << "queryDialog::on_queryOkbutton_clicked: chosen label " << chosenLabel << endl;
#endif
	if (chosenLabel == 0)
	{
		// No label
		m_properties.setLabelName("");
	}
	else
	{
		TreeModel::iterator iter = labelCombobox->get_active();
		TreeModel::Row row = *iter;
		string labelName = locale_from_utf8(row[m_labelColumns.m_name]);

#ifdef DEBUG
		cout << "queryDialog::on_queryOkbutton_clicked: label is " << labelName << endl;
#endif
		m_properties.setLabelName(labelName);
	}
}

void queryDialog::on_nameEntry_changed()
{
	ustring name = nameEntry->get_text();
	if (name.empty() == false)
	{
		queryOkbutton->set_sensitive(true);
	}
	else
	{
		queryOkbutton->set_sensitive(false);
	}
}

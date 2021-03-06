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

#include <stdio.h>
#include <iostream>
#include <utility>

#include "config.h"
#include "Languages.h"
#include "NLS.h"
#include "PinotSettings.h"
#include "PinotUtils.h"
#include "propertiesDialog.hh"

using namespace std;
using namespace Glib;
using namespace Gtk;

propertiesDialog::propertiesDialog(const std::set<std::string> &docLabels,
	const DocumentInfo &docInfo, unsigned int termsCount, bool editDocument) :
	propertiesDialog_glade(),
	m_editDocument(editDocument),
	m_docInfo(docInfo)
{
	string language(docInfo.getLanguage());
	char numStr[128];
	unsigned int indexId = 0;
	bool notALanguageName = false;

	// Get the document ID
	unsigned int docId = docInfo.getIsIndexed(indexId);
#ifdef DEBUG
	cout << "propertiesDialog: document " << docId << endl;
#endif
	if (docId > 0)
	{
		snprintf(numStr, 128, "%u", docId);
		set_title(get_title() + " (ID " + numStr + ")");
	}

	// Associate the columns model to the labels tree
	m_refLabelsTree = ListStore::create(m_labelsColumns);
	labelsTreeview->set_model(m_refLabelsTree);
	labelsTreeview->append_column_editable(" ", m_labelsColumns.m_enabled);
	labelsTreeview->append_column(_("Label"), m_labelsColumns.m_name);
	// Allow only single selection
	labelsTreeview->get_selection()->set_mode(SELECTION_SINGLE);

	if (m_editDocument == true)
	{
		unsigned int size = docInfo.getSize();

		titleEntry->set_text(to_utf8(docInfo.getTitle()));
		typeEntry->set_text(to_utf8(docInfo.getType()));
		if (size == 0)
		{
			sizeEntry->set_text(_("Unknown"));
		}
		else
		{
			snprintf(numStr, 128, "%u", size);
			sizeEntry->set_text(numStr);
		}
		if (termsCount == 0)
		{
			termsEntry->set_text(_("Unknown"));
		}
		else
		{
			snprintf(numStr, 128, "%u", termsCount);
			termsEntry->set_text(numStr);
		}
	}
	else
	{
		if (language.empty() == true)
		{
			language = _("Per document"); 
			notALanguageName = true;
		}

		titleLabel->hide();
		titleEntry->hide();
		typeLabel->hide();
		typeEntry->hide();
		sizeLabel->hide();
		sizeEntry->hide();
		termsLabel->hide();
		termsEntry->hide();
	}

	populate_languageCombobox(language, notALanguageName);
	populate_labelsTreeview(docLabels);
}

propertiesDialog::~propertiesDialog()
{
}

void propertiesDialog::populate_languageCombobox(const string &language, bool notALanguageName)
{
	unsigned int languageStart = 0;
	bool foundLanguage = false;

	if (notALanguageName == true)
	{
		languageCombobox->append_text(to_utf8(language));
		languageCombobox->set_active(0);
		languageStart = 1;
	}

	// Add all supported languages
	for (unsigned int languageNum = 0; languageNum < Languages::m_count; ++languageNum)
	{
		string languageName(Languages::getIntlName(languageNum));

		languageCombobox->append_text(to_utf8(languageName));
		if ((notALanguageName == false) &&
			(languageName == language))
		{
			languageCombobox->set_active(languageNum + languageStart);
			foundLanguage = true;
		}
	}

	// Did we find the given language ?
	if ((notALanguageName == false) &&
		(foundLanguage == false))
	{
		// Select the first language in the list
		languageCombobox->set_active(languageStart);
	}
}

void propertiesDialog::populate_labelsTreeview(const set<string> &docLabels)
{
	TreeModel::iterator iter;
	TreeModel::Row row;

	// Populate the tree
	const set<string> &sysLabels = PinotSettings::getInstance().getLabels();
	for (set<string>::const_iterator labelIter = sysLabels.begin(); labelIter != sysLabels.end(); ++labelIter)
	{
		string labelName(*labelIter);

		// Create a new row
		iter = m_refLabelsTree->append();
		row = *iter;

		row[m_labelsColumns.m_name] = labelName;
		// Is it in the document labels list ?
		set<string>::const_iterator iter = find(docLabels.begin(), docLabels.end(), labelName);
		if (iter != docLabels.end())
		{
			// Yup
			row[m_labelsColumns.m_enabled] = true;
		}
		else
		{
			row[m_labelsColumns.m_enabled] = false;
		}
	}
}

void propertiesDialog::setHeight(int maxHeight)
{
	// FIXME: there must be a better way to determine how high the tree should be
	// for all rows to be visible !
	int rowsCount = m_refLabelsTree->children().size();
	// By default, the tree is high enough for two rows
	if (rowsCount > 2)
	{
		int width, height;

		// What's the current size ?
		get_size(width, height);
		// Add enough room for the rows we need to show
		height += get_column_height(labelsTreeview) * (rowsCount - 1);
		// Resize
		resize(width, min(maxHeight, height));
	}
}

const DocumentInfo &propertiesDialog::getDocumentInfo(void)
{
	return m_docInfo;
}

const set<string> &propertiesDialog::getLabels(void) const
{
	return m_labels;
}

void propertiesDialog::on_labelOkButton_clicked()
{
	unsigned int languageStart = 0;
	if (m_editDocument == true)
	{
		// Title
		m_docInfo.setTitle(from_utf8(titleEntry->get_text()));
	}

	// Did we add an extra string to the languages list ?
	if (m_docInfo.getLanguage().empty() == true)
	{
		languageStart = 1;
	}
	int chosenLanguage = languageCombobox->get_active_row_number();
	if (chosenLanguage >= languageStart)
	{
		m_docInfo.setLanguage(from_utf8(languageCombobox->get_active_text()));
	}

	// Go through the labels tree
	TreeModel::Children children = m_refLabelsTree->children();
	if (children.empty() == false)
	{
		for (TreeModel::Children::iterator iter = children.begin(); iter != children.end(); ++iter)
		{
			TreeModel::Row row = *iter;

			bool enabled = row[m_labelsColumns.m_enabled];
			if (enabled == true)
			{
				ustring labelName = row[m_labelsColumns.m_name];
				m_labels.insert(from_utf8(labelName));
			}
		}
	}
}

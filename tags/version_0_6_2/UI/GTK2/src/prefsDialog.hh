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

#ifndef _PREFSDIALOG_HH
#define _PREFSDIALOG_HH

#include <string>
#include <map>
#include <set>
#include <glibmm/refptr.h>
#include <glibmm/ustring.h>
#include <gtkmm/liststore.h>

#include "ModelColumns.h"
#include "PinotSettings.h"
#include "prefsDialog_glade.hh"

class prefsDialog : public prefsDialog_glade
{  
public:
	prefsDialog();
	virtual ~prefsDialog();

	const std::set<std::string> &getLabelsToDelete(void) const;

	const std::map<std::string, std::string> &getLabelsToRename(void) const;

protected:
	virtual void on_prefsOkbutton_clicked();
	virtual void on_addLabelButton_clicked();
	virtual void on_removeLabelButton_clicked();
	virtual void on_addDirectoryButton_clicked();
	virtual void on_removeDirectoryButton_clicked();
	virtual void on_addAccountButton_clicked();
	virtual void on_removeAccountButton_clicked();
	virtual void on_addPatternButton_clicked();
	virtual void on_removePatternButton_clicked();

	void populate_labelsTreeview();
	void save_labelsTreeview();
	void populate_directoriesTreeview();
	bool save_directoriesTreeview();
	void populate_mailTreeview();
	bool save_mailTreeview();
	void populate_patternsTreeview();
	void save_patternsTreeview();

private:
	PinotSettings &m_settings;
	ComboModelColumns m_viewColumns;
	Glib::RefPtr<Gtk::ListStore> m_refViewTree;
	LabelModelColumns m_labelsColumns;
	Glib::RefPtr<Gtk::ListStore> m_refLabelsTree;
	IndexableModelColumns m_directoriesColumns;
	Glib::RefPtr<Gtk::ListStore> m_refDirectoriesTree;
	TimestampedModelColumns m_mailColumns;
	Glib::RefPtr<Gtk::ListStore> m_refMailTree;
	TimestampedModelColumns m_patternsColumns;
	Glib::RefPtr<Gtk::ListStore> m_refPatternsTree;
	std::set<std::string> m_deletedLabels;
	std::map<std::string, std::string> m_renamedLabels;
	std::set<std::string> m_deletedDirectories;
	std::set<std::string> m_deletedMail;

};
#endif

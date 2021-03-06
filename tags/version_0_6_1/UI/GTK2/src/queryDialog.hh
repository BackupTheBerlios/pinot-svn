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

#ifndef _QUERYDIALOG_HH
#define _QUERYDIALOG_HH

#include <string>
#include <set>
#include <gtkmm/liststore.h>

#include "QueryProperties.h"
#include "ModelColumns.h"
#include "PinotSettings.h"
#include "queryDialog_glade.hh"

class queryDialog : public queryDialog_glade
{
public:
	queryDialog(QueryProperties &properties);
	virtual ~queryDialog();

	bool badName(void) const;

protected:
	std::string m_name;
	QueryProperties& m_properties;
	const std::set<std::string> &m_labels;
	ComboModelColumns m_labelNameColumns;
	Glib::RefPtr<Gtk::ListStore> m_refLabelNameTree;
	ComboModelColumns m_filterColumns;
	Glib::RefPtr<Gtk::ListStore> m_refFilterTree;
	bool m_badName;

	void populate_comboboxes();

	virtual void on_queryOkbutton_clicked();
	virtual void on_nameEntry_changed();
	virtual void on_addFilterButton_clicked();

};
#endif

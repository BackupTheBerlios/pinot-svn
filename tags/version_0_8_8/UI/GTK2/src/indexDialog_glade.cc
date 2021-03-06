// generated 2007/3/22 1:27:17 SGT by fabrice@amra.dyndns.org.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/fabrice/Projects/MetaSE/pinot/UI/GTK2/metase-gtk2.glade
// for gtk 2.10.8 and gtkmm 2.10.7
//
// Please modify the corresponding derived classes in ./src/indexDialog.cc


#if defined __GNUC__ && __GNUC__ < 3
#error This program will crash if compiled with g++ 2.x
// see the dynamic_cast bug in the gtkmm FAQ
#endif //
#include "config.h"
/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (GETTEXT_PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#endif
#include <gtkmmconfig.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#include <sigc++/sigc++.h>
#define GMM_GTKMM_22_24(a,b) b
#else //gtkmm 2.2
#define GMM_GTKMM_22_24(a,b) a
#endif //
#include "indexDialog_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/table.h>
#ifndef ENABLE_NLS
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif


indexDialog_glade::indexDialog_glade(
)
{  
   
   Gtk::Dialog *indexDialog = this;
   gmm_data = new GlademmData(get_accel_group());
   indexCancelbutton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
   indexOkbutton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   
   Gtk::Label *nameLabel = Gtk::manage(new class Gtk::Label(_("Name:")));
   locationEntry = Gtk::manage(new class Gtk::Entry());
   locationButton = Gtk::manage(new class Gtk::Button(_("...")));
   
   Gtk::HBox *locationHbox = Gtk::manage(new class Gtk::HBox(false, 4));
   Gtk::Label *typeLabel = Gtk::manage(new class Gtk::Label(_("Type:")));
   Gtk::Label *locationLabel = Gtk::manage(new class Gtk::Label(_("Location:")));
   Gtk::Label *portLabel = Gtk::manage(new class Gtk::Label(_("Port:")));
   nameEntry = Gtk::manage(new class Gtk::Entry());
   
   Gtk::Adjustment *portSpinbutton_adj = Gtk::manage(new class Gtk::Adjustment(1024, 1, 65535, 1, 10, 10));
   portSpinbutton = Gtk::manage(new class Gtk::SpinButton(*portSpinbutton_adj, 1, 0));
   typeCombobox = Gtk::manage(new class Gtk::ComboBoxText());
   
   Gtk::Label *hostLabel = Gtk::manage(new class Gtk::Label(_("Address:")));
   hostEntry = Gtk::manage(new class Gtk::Entry());
   
   Gtk::Table *indexTable = Gtk::manage(new class Gtk::Table(2, 2, false));
   indexCancelbutton->set_flags(Gtk::CAN_FOCUS);
   indexCancelbutton->set_flags(Gtk::CAN_DEFAULT);
   indexCancelbutton->set_relief(Gtk::RELIEF_NORMAL);
   indexOkbutton->set_flags(Gtk::CAN_FOCUS);
   indexOkbutton->set_flags(Gtk::CAN_DEFAULT);
   indexOkbutton->set_relief(Gtk::RELIEF_NORMAL);
   indexDialog->get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
   nameLabel->set_alignment(0,0.5);
   nameLabel->set_padding(4,4);
   nameLabel->set_justify(Gtk::JUSTIFY_LEFT);
   nameLabel->set_line_wrap(false);
   nameLabel->set_use_markup(false);
   nameLabel->set_selectable(false);
   nameLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   nameLabel->set_width_chars(-1);
   nameLabel->set_angle(0);
   nameLabel->set_single_line_mode(false);
   locationEntry->set_flags(Gtk::CAN_FOCUS);
   locationEntry->set_visibility(true);
   locationEntry->set_editable(true);
   locationEntry->set_max_length(0);
   locationEntry->set_has_frame(true);
   locationEntry->set_activates_default(false);
   locationButton->set_flags(Gtk::CAN_FOCUS);
   locationButton->set_flags(Gtk::CAN_DEFAULT);
   locationButton->set_relief(Gtk::RELIEF_NORMAL);
   locationHbox->pack_start(*locationEntry);
   locationHbox->pack_start(*locationButton, Gtk::PACK_SHRINK, 0);
   typeLabel->set_alignment(0,0.5);
   typeLabel->set_padding(4,4);
   typeLabel->set_justify(Gtk::JUSTIFY_LEFT);
   typeLabel->set_line_wrap(false);
   typeLabel->set_use_markup(false);
   typeLabel->set_selectable(false);
   typeLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   typeLabel->set_width_chars(-1);
   typeLabel->set_angle(0);
   typeLabel->set_single_line_mode(false);
   locationLabel->set_alignment(0,0.5);
   locationLabel->set_padding(4,4);
   locationLabel->set_justify(Gtk::JUSTIFY_LEFT);
   locationLabel->set_line_wrap(false);
   locationLabel->set_use_markup(false);
   locationLabel->set_selectable(false);
   locationLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   locationLabel->set_width_chars(-1);
   locationLabel->set_angle(0);
   locationLabel->set_single_line_mode(false);
   portLabel->set_alignment(0,0.5);
   portLabel->set_padding(4,4);
   portLabel->set_justify(Gtk::JUSTIFY_LEFT);
   portLabel->set_line_wrap(false);
   portLabel->set_use_markup(false);
   portLabel->set_selectable(false);
   portLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   portLabel->set_width_chars(-1);
   portLabel->set_angle(0);
   portLabel->set_single_line_mode(false);
   nameEntry->set_flags(Gtk::CAN_FOCUS);
   nameEntry->set_visibility(true);
   nameEntry->set_editable(true);
   nameEntry->set_max_length(0);
   nameEntry->set_has_frame(true);
   nameEntry->set_activates_default(false);
   portSpinbutton->set_flags(Gtk::CAN_FOCUS);
   portSpinbutton->set_update_policy(Gtk::UPDATE_ALWAYS);
   portSpinbutton->set_numeric(false);
   portSpinbutton->set_digits(0);
   portSpinbutton->set_wrap(false);
   hostLabel->set_alignment(0,0.5);
   hostLabel->set_padding(4,4);
   hostLabel->set_justify(Gtk::JUSTIFY_LEFT);
   hostLabel->set_line_wrap(false);
   hostLabel->set_use_markup(false);
   hostLabel->set_selectable(false);
   hostLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   hostLabel->set_width_chars(-1);
   hostLabel->set_angle(0);
   hostLabel->set_single_line_mode(false);
   hostEntry->set_flags(Gtk::CAN_FOCUS);
   hostEntry->set_visibility(true);
   hostEntry->set_editable(true);
   hostEntry->set_max_length(0);
   hostEntry->set_has_frame(true);
   hostEntry->set_activates_default(false);
   indexTable->set_row_spacings(0);
   indexTable->set_col_spacings(0);
   indexTable->attach(*nameLabel, 0, 1, 0, 1, Gtk::FILL, Gtk::FILL, 0, 0);
   indexTable->attach(*locationHbox, 1, 2, 2, 3, Gtk::FILL, Gtk::FILL, 4, 4);
   indexTable->attach(*typeLabel, 0, 1, 1, 2, Gtk::FILL, Gtk::FILL, 0, 0);
   indexTable->attach(*locationLabel, 0, 1, 2, 3, Gtk::FILL, Gtk::FILL, 0, 0);
   indexTable->attach(*portLabel, 0, 1, 4, 5, Gtk::FILL, Gtk::FILL, 0, 0);
   indexTable->attach(*nameEntry, 1, 2, 0, 1, Gtk::EXPAND|Gtk::FILL, Gtk::AttachOptions(), 4, 4);
   indexTable->attach(*portSpinbutton, 1, 2, 4, 5, Gtk::EXPAND|Gtk::FILL, Gtk::AttachOptions(), 4, 4);
   indexTable->attach(*typeCombobox, 1, 2, 1, 2, Gtk::EXPAND|Gtk::FILL, Gtk::AttachOptions(), 4, 4);
   indexTable->attach(*hostLabel, 0, 1, 3, 4, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   indexTable->attach(*hostEntry, 1, 2, 3, 4, Gtk::EXPAND|Gtk::FILL, Gtk::AttachOptions(), 4, 4);
   indexDialog->get_vbox()->set_homogeneous(false);
   indexDialog->get_vbox()->set_spacing(0);
   indexDialog->get_vbox()->pack_start(*indexTable);
   indexDialog->set_title(_("External index"));
   indexDialog->set_modal(false);
   indexDialog->property_window_position().set_value(Gtk::WIN_POS_NONE);
   indexDialog->set_resizable(true);
   indexDialog->property_destroy_with_parent().set_value(false);
   indexDialog->set_has_separator(true);
   indexDialog->add_action_widget(*indexCancelbutton, -6);
   indexDialog->add_action_widget(*indexOkbutton, -5);
   indexCancelbutton->show();
   indexOkbutton->show();
   nameLabel->show();
   locationEntry->show();
   locationButton->show();
   locationHbox->show();
   typeLabel->show();
   locationLabel->show();
   portLabel->show();
   nameEntry->show();
   portSpinbutton->show();
   typeCombobox->show();
   hostLabel->show();
   hostEntry->show();
   indexTable->show();
   indexDialog->show();
   indexOkbutton->signal_clicked().connect(sigc::mem_fun(*this, &indexDialog_glade::on_indexOkbutton_clicked), false);
   locationEntry->signal_changed().connect(sigc::mem_fun(*this, &indexDialog_glade::on_locationEntry_changed), false);
   locationButton->signal_clicked().connect(sigc::mem_fun(*this, &indexDialog_glade::on_locationButton_clicked), false);
   nameEntry->signal_changed().connect(sigc::mem_fun(*this, &indexDialog_glade::on_nameEntry_changed), false);
   typeCombobox->signal_changed().connect(sigc::mem_fun(*this, &indexDialog_glade::on_typeCombobox_changed), false);
   hostEntry->signal_changed().connect(sigc::mem_fun(*this, &indexDialog_glade::on_nameEntry_changed), false);
}

indexDialog_glade::~indexDialog_glade()
{  delete gmm_data;
}

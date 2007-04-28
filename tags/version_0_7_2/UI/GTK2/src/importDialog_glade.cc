// generated 2006/8/20 15:34:18 SGT by fabrice@amra.dyndns.org.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/fabrice/Projects/MetaSE/pinot/UI/GTK2/metase-gtk2.glade
// for gtk 2.8.20 and gtkmm 2.8.8
//
// Please modify the corresponding derived classes in ./src/importDialog.cc


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
#include <sigc++/compatibility.h>
#include <sigc++/bind.h>
#define GMM_GTKMM_22_24(a,b) b
#else //gtkmm 2.2
#define GMM_GTKMM_22_24(a,b) a
#endif //
#include "importDialog_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/label.h>
#include <gtkmm/image.h>
#include <gtkmm/box.h>
#include <gtkmm/alignment.h>
#ifndef ENABLE_NLS
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif


importDialog_glade::importDialog_glade(
)
{  
   
   Gtk::Dialog *importDialog = this;
   gmm_data = new GlademmData(get_accel_group());
   
   Gtk::Button *closeButton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-close")));
   titleEntry = Gtk::manage(new class Gtk::Entry());
   
   Gtk::Label *locationLabel = Gtk::manage(new class Gtk::Label(_("Location:")));
   titleLabel = Gtk::manage(new class Gtk::Label(_("Title:")));
   labelNameCombobox = Gtk::manage(new class Gtk::ComboBoxText());
   
   Gtk::Label *labelNameLabel = Gtk::manage(new class Gtk::Label(_("Apply label:")));
   locationEntry = Gtk::manage(new class Gtk::Entry());
   docTable = Gtk::manage(new class Gtk::Table(2, 2, false));
   importProgressbar = Gtk::manage(new class Gtk::ProgressBar());
   
   Gtk::Image *image575 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-open"), Gtk::IconSize(4)));
   Gtk::Label *label55 = Gtk::manage(new class Gtk::Label(_("Import")));
   Gtk::HBox *hbox52 = Gtk::manage(new class Gtk::HBox(false, 2));
   Gtk::Alignment *alignment34 = Gtk::manage(new class Gtk::Alignment(0.5, 0.5, 0, 0));
   importButton = Gtk::manage(new class Gtk::Button());
   
   Gtk::HBox *importHbox = Gtk::manage(new class Gtk::HBox(false, 4));
   Gtk::VBox *importVbox = Gtk::manage(new class Gtk::VBox(false, 0));
   closeButton->set_flags(Gtk::CAN_FOCUS);
   closeButton->set_flags(Gtk::CAN_DEFAULT);
   closeButton->set_relief(Gtk::RELIEF_NORMAL);
   importDialog->get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
   titleEntry->set_flags(Gtk::CAN_FOCUS);
   titleEntry->set_visibility(true);
   titleEntry->set_editable(true);
   titleEntry->set_max_length(0);
   titleEntry->set_has_frame(true);
   titleEntry->set_activates_default(false);
   locationLabel->set_alignment(0,0.5);
   locationLabel->set_padding(4,4);
   locationLabel->set_justify(Gtk::JUSTIFY_LEFT);
   locationLabel->set_line_wrap(false);
   locationLabel->set_use_markup(false);
   locationLabel->set_selectable(false);
   titleLabel->set_alignment(0,0.5);
   titleLabel->set_padding(4,4);
   titleLabel->set_justify(Gtk::JUSTIFY_LEFT);
   titleLabel->set_line_wrap(false);
   titleLabel->set_use_markup(false);
   titleLabel->set_selectable(false);
   labelNameLabel->set_alignment(0,0.5);
   labelNameLabel->set_padding(4,4);
   labelNameLabel->set_justify(Gtk::JUSTIFY_LEFT);
   labelNameLabel->set_line_wrap(false);
   labelNameLabel->set_use_markup(false);
   labelNameLabel->set_selectable(false);
   locationEntry->set_flags(Gtk::CAN_FOCUS);
   locationEntry->set_visibility(true);
   locationEntry->set_editable(true);
   locationEntry->set_max_length(0);
   locationEntry->set_has_frame(true);
   locationEntry->set_activates_default(false);
   docTable->set_row_spacings(0);
   docTable->set_col_spacings(0);
   docTable->attach(*titleEntry, 1, 2, 0, 1, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   docTable->attach(*locationLabel, 0, 1, 1, 2, Gtk::FILL, Gtk::FILL, 0, 0);
   docTable->attach(*titleLabel, 0, 1, 0, 1, Gtk::FILL, Gtk::FILL, 0, 0);
   docTable->attach(*labelNameCombobox, 1, 2, 2, 3, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   docTable->attach(*labelNameLabel, 0, 1, 2, 3, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   docTable->attach(*locationEntry, 1, 2, 1, 2, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   image575->set_alignment(0.5,0.5);
   image575->set_padding(0,0);
   label55->set_alignment(0.5,0.5);
   label55->set_padding(0,0);
   label55->set_justify(Gtk::JUSTIFY_LEFT);
   label55->set_line_wrap(false);
   label55->set_use_markup(false);
   label55->set_selectable(false);
   hbox52->pack_start(*image575, Gtk::PACK_SHRINK, 0);
   hbox52->pack_start(*label55, Gtk::PACK_SHRINK, 0);
   alignment34->add(*hbox52);
   importButton->set_flags(Gtk::CAN_FOCUS);
   importButton->set_flags(Gtk::CAN_DEFAULT);
   importButton->set_relief(Gtk::RELIEF_NORMAL);
   importButton->add(*alignment34);
   importHbox->pack_start(*importProgressbar);
   importHbox->pack_start(*importButton, Gtk::PACK_SHRINK, 0);
   importVbox->pack_start(*docTable, Gtk::PACK_SHRINK, 0);
   importVbox->pack_start(*importHbox, Gtk::PACK_SHRINK, 4);
   importDialog->get_vbox()->set_homogeneous(false);
   importDialog->get_vbox()->set_spacing(0);
   importDialog->get_vbox()->pack_start(*importVbox);
   importDialog->set_title(_("Import URL"));
   importDialog->set_modal(false);
   importDialog->property_window_position().set_value(Gtk::WIN_POS_NONE);
   importDialog->set_resizable(true);
   importDialog->property_destroy_with_parent().set_value(false);
   importDialog->set_has_separator(true);
   importDialog->add_action_widget(*closeButton, -5);
   closeButton->show();
   titleEntry->show();
   locationLabel->show();
   titleLabel->show();
   labelNameCombobox->show();
   labelNameLabel->show();
   locationEntry->show();
   docTable->show();
   importProgressbar->show();
   image575->show();
   label55->show();
   hbox52->show();
   alignment34->show();
   importButton->show();
   importHbox->show();
   importVbox->show();
   locationEntry->signal_changed().connect(SigC::slot(*this, &importDialog_glade::on_locationEntry_changed), false);
   importButton->signal_clicked().connect(SigC::slot(*this, &importDialog_glade::on_importButton_clicked), false);
   importDialog->signal_response().connect(SigC::slot(*this, &importDialog_glade::on_importDialog_response), false);
}

importDialog_glade::~importDialog_glade()
{  delete gmm_data;
}

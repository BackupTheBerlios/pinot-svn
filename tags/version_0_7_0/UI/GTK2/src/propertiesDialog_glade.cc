// generated 2007/1/22 22:28:26 SGT by fabrice@amra.dyndns.org.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/fabrice/Projects/MetaSE/pinot/UI/GTK2/metase-gtk2.glade
// for gtk 2.10.4 and gtkmm 2.10.5
//
// Please modify the corresponding derived classes in ./src/propertiesDialog.cc


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
#include "propertiesDialog_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/table.h>
#include <gtkmm/viewport.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/box.h>
#ifndef ENABLE_NLS
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif


propertiesDialog_glade::propertiesDialog_glade(
)
{  propertiesDialog = this;
   gmm_data = new GlademmData(get_accel_group());
   
   Gtk::Button *cancelbutton2 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
   labelOkButton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   titleEntry = Gtk::manage(new class Gtk::Entry());
   typeEntry = Gtk::manage(new class Gtk::Entry());
   languageCombobox = Gtk::manage(new class Gtk::ComboBox());
   titleLabel = Gtk::manage(new class Gtk::Label(_("Title:")));
   languageLabel = Gtk::manage(new class Gtk::Label(_("Language:")));
   typeLabel = Gtk::manage(new class Gtk::Label(_("MIME type:")));
   sizeLabel = Gtk::manage(new class Gtk::Label(_("Size:")));
   sizeEntry = Gtk::manage(new class Gtk::Entry());
   termsEntry = Gtk::manage(new class Gtk::Entry());
   termsLabel = Gtk::manage(new class Gtk::Label(_("Terms:")));
   
   Gtk::Table *propertiesTable = Gtk::manage(new class Gtk::Table(2, 2, false));
   labelsTreeview = Gtk::manage(new class Gtk::TreeView());
   
   Gtk::Viewport *viewport1 = Gtk::manage(new class Gtk::Viewport(*manage(new Gtk::Adjustment(0,0,1)), *manage(new Gtk::Adjustment(0,0,1))));
   labelsScrolledwindow = Gtk::manage(new class Gtk::ScrolledWindow());
   
   Gtk::VBox *propertiesVbox = Gtk::manage(new class Gtk::VBox(false, 0));
   cancelbutton2->set_flags(Gtk::CAN_FOCUS);
   cancelbutton2->set_flags(Gtk::CAN_DEFAULT);
   cancelbutton2->set_relief(Gtk::RELIEF_NORMAL);
   labelOkButton->set_flags(Gtk::CAN_FOCUS);
   labelOkButton->set_flags(Gtk::CAN_DEFAULT);
   labelOkButton->set_relief(Gtk::RELIEF_NORMAL);
   propertiesDialog->get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
   titleEntry->set_flags(Gtk::CAN_FOCUS);
   titleEntry->set_visibility(true);
   titleEntry->set_editable(true);
   titleEntry->set_max_length(0);
   titleEntry->set_has_frame(true);
   titleEntry->set_activates_default(false);
   typeEntry->set_flags(Gtk::CAN_FOCUS);
   typeEntry->set_visibility(true);
   typeEntry->set_editable(false);
   typeEntry->set_max_length(0);
   typeEntry->set_has_frame(true);
   typeEntry->set_activates_default(false);
   titleLabel->set_alignment(0,0.5);
   titleLabel->set_padding(0,0);
   titleLabel->set_justify(Gtk::JUSTIFY_LEFT);
   titleLabel->set_line_wrap(false);
   titleLabel->set_use_markup(false);
   titleLabel->set_selectable(false);
   languageLabel->set_alignment(0,0.5);
   languageLabel->set_padding(0,0);
   languageLabel->set_justify(Gtk::JUSTIFY_LEFT);
   languageLabel->set_line_wrap(false);
   languageLabel->set_use_markup(false);
   languageLabel->set_selectable(false);
   typeLabel->set_alignment(0,0.5);
   typeLabel->set_padding(0,0);
   typeLabel->set_justify(Gtk::JUSTIFY_LEFT);
   typeLabel->set_line_wrap(false);
   typeLabel->set_use_markup(false);
   typeLabel->set_selectable(false);
   sizeLabel->set_alignment(0,0.5);
   sizeLabel->set_padding(0,0);
   sizeLabel->set_justify(Gtk::JUSTIFY_LEFT);
   sizeLabel->set_line_wrap(false);
   sizeLabel->set_use_markup(false);
   sizeLabel->set_selectable(false);
   sizeEntry->set_flags(Gtk::CAN_FOCUS);
   sizeEntry->set_visibility(true);
   sizeEntry->set_editable(false);
   sizeEntry->set_max_length(0);
   sizeEntry->set_has_frame(true);
   sizeEntry->set_activates_default(false);
   termsEntry->set_flags(Gtk::CAN_FOCUS);
   termsEntry->set_visibility(true);
   termsEntry->set_editable(false);
   termsEntry->set_max_length(0);
   termsEntry->set_has_frame(true);
   termsEntry->set_activates_default(false);
   termsLabel->set_alignment(0,0.5);
   termsLabel->set_padding(0,0);
   termsLabel->set_justify(Gtk::JUSTIFY_LEFT);
   termsLabel->set_line_wrap(false);
   termsLabel->set_use_markup(false);
   termsLabel->set_selectable(false);
   propertiesTable->set_row_spacings(0);
   propertiesTable->set_col_spacings(0);
   propertiesTable->attach(*titleEntry, 1, 2, 0, 1, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   propertiesTable->attach(*typeEntry, 1, 2, 2, 3, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   propertiesTable->attach(*languageCombobox, 1, 2, 1, 2, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   propertiesTable->attach(*titleLabel, 0, 1, 0, 1, Gtk::FILL, Gtk::FILL, 4, 4);
   propertiesTable->attach(*languageLabel, 0, 1, 1, 2, Gtk::FILL, Gtk::FILL, 4, 4);
   propertiesTable->attach(*typeLabel, 0, 1, 2, 3, Gtk::FILL, Gtk::FILL, 4, 4);
   propertiesTable->attach(*sizeLabel, 0, 1, 3, 4, Gtk::FILL, Gtk::FILL, 4, 4);
   propertiesTable->attach(*sizeEntry, 1, 2, 3, 4, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   propertiesTable->attach(*termsEntry, 1, 2, 4, 5, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   propertiesTable->attach(*termsLabel, 0, 1, 4, 5, Gtk::FILL, Gtk::FILL, 4, 4);
   labelsTreeview->set_flags(Gtk::CAN_FOCUS);
   labelsTreeview->set_headers_visible(true);
   labelsTreeview->set_rules_hint(false);
   labelsTreeview->set_reorderable(false);
   labelsTreeview->set_enable_search(true);
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=5
   labelsTreeview->set_fixed_height_mode(false);
   labelsTreeview->set_hover_selection(false);
   labelsTreeview->set_hover_expand(false);
#endif //
   viewport1->set_shadow_type(Gtk::SHADOW_IN);
   viewport1->add(*labelsTreeview);
   labelsScrolledwindow->set_flags(Gtk::CAN_FOCUS);
   labelsScrolledwindow->set_shadow_type(Gtk::SHADOW_NONE);
   labelsScrolledwindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   labelsScrolledwindow->property_window_placement().set_value(Gtk::CORNER_TOP_LEFT);
   labelsScrolledwindow->add(*viewport1);
   propertiesVbox->pack_start(*propertiesTable, Gtk::PACK_SHRINK, 0);
   propertiesVbox->pack_start(*labelsScrolledwindow);
   propertiesDialog->get_vbox()->set_homogeneous(false);
   propertiesDialog->get_vbox()->set_spacing(0);
   propertiesDialog->get_vbox()->pack_start(*propertiesVbox);
   propertiesDialog->set_title(_("Properties"));
   propertiesDialog->set_modal(false);
   propertiesDialog->property_window_position().set_value(Gtk::WIN_POS_NONE);
   propertiesDialog->set_resizable(true);
   propertiesDialog->property_destroy_with_parent().set_value(false);
   propertiesDialog->set_has_separator(true);
   propertiesDialog->add_action_widget(*cancelbutton2, -6);
   propertiesDialog->add_action_widget(*labelOkButton, -5);
   cancelbutton2->show();
   labelOkButton->show();
   titleEntry->show();
   typeEntry->show();
   languageCombobox->show();
   titleLabel->show();
   languageLabel->show();
   typeLabel->show();
   sizeLabel->show();
   sizeEntry->show();
   termsEntry->show();
   termsLabel->show();
   propertiesTable->show();
   labelsTreeview->show();
   viewport1->show();
   labelsScrolledwindow->show();
   propertiesVbox->show();
   labelOkButton->signal_clicked().connect(SigC::slot(*this, &propertiesDialog_glade::on_labelOkButton_clicked), false);
}

propertiesDialog_glade::~propertiesDialog_glade()
{  delete gmm_data;
}

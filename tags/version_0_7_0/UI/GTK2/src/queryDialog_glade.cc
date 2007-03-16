// generated 2007/1/11 21:02:42 SGT by fabrice@amra.dyndns.org.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/fabrice/Projects/MetaSE/pinot/UI/GTK2/metase-gtk2.glade
// for gtk 2.10.4 and gtkmm 2.10.5
//
// Please modify the corresponding derived classes in ./src/queryDialog.cc


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
#include "queryDialog_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/label.h>
#include <gtkmm/table.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/alignment.h>
#include <gtkmm/frame.h>
#include <gtkmm/adjustment.h>
#ifndef ENABLE_NLS
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif


queryDialog_glade::queryDialog_glade(
)
{  queryDialog = this;
   gmm_data = new GlademmData(get_accel_group());
   
   Gtk::Button *queryCancelbutton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
   queryOkbutton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   
   Gtk::Label *nameLabel = Gtk::manage(new class Gtk::Label(_("Name:")));
   nameEntry = Gtk::manage(new class Gtk::Entry());
   
   Gtk::Table *nameTable = Gtk::manage(new class Gtk::Table(2, 2, false));
   Gtk::Label *filterLabel = Gtk::manage(new class Gtk::Label(_("New filter:")));
   filterCombobox = Gtk::manage(new class Gtk::ComboBox());
   
   Gtk::Button *addFilterButton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-add")));
   Gtk::HBox *queryHbox = Gtk::manage(new class Gtk::HBox(false, 0));
   queryTextview = Gtk::manage(new class Gtk::TextView());
   
   Gtk::ScrolledWindow *queryScrolledwindow = Gtk::manage(new class Gtk::ScrolledWindow());
   Gtk::Alignment *queryFrameAlignment = Gtk::manage(new class Gtk::Alignment(0.5, 0.5, 1, 1));
   Gtk::Label *queryFrameLabel = Gtk::manage(new class Gtk::Label(_("Query Text")));
   Gtk::Frame *queryFrame = Gtk::manage(new class Gtk::Frame());
   indexCheckbutton = Gtk::manage(new class Gtk::CheckButton(_("Index results with label")));
   labelNameCombobox = Gtk::manage(new class Gtk::ComboBox());
   
   Gtk::Label *resultsCountLabel = Gtk::manage(new class Gtk::Label(_("Number of results:")));
   Gtk::Adjustment *resultsCountSpinbutton_adj = Gtk::manage(new class Gtk::Adjustment(10, 10, 100, 10, 20, 20));
   resultsCountSpinbutton = Gtk::manage(new class Gtk::SpinButton(*resultsCountSpinbutton_adj, 1, 0));
   fromCheckbutton = Gtk::manage(new class Gtk::CheckButton(_("Start date:")));
   toCheckbutton = Gtk::manage(new class Gtk::CheckButton(_("End date:")));
   fromButton = Gtk::manage(new class Gtk::Button(_("Date")));
   toButton = Gtk::manage(new class Gtk::Button(_("Date")));
   
   Gtk::Table *actionsTable = Gtk::manage(new class Gtk::Table(2, 2, false));
   Gtk::VBox *queryVbox = Gtk::manage(new class Gtk::VBox(false, 0));
   queryCancelbutton->set_flags(Gtk::CAN_FOCUS);
   queryCancelbutton->set_flags(Gtk::CAN_DEFAULT);
   queryCancelbutton->set_relief(Gtk::RELIEF_NORMAL);
   queryOkbutton->set_flags(Gtk::CAN_FOCUS);
   queryOkbutton->set_flags(Gtk::CAN_DEFAULT);
   queryOkbutton->set_relief(Gtk::RELIEF_NORMAL);
   queryDialog->get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
   nameLabel->set_alignment(0.5,0.5);
   nameLabel->set_padding(4,4);
   nameLabel->set_justify(Gtk::JUSTIFY_LEFT);
   nameLabel->set_line_wrap(false);
   nameLabel->set_use_markup(false);
   nameLabel->set_selectable(false);
   nameEntry->set_flags(Gtk::CAN_FOCUS);
   nameEntry->set_visibility(true);
   nameEntry->set_editable(true);
   nameEntry->set_max_length(0);
   nameEntry->set_has_frame(true);
   nameEntry->set_activates_default(false);
   nameTable->set_row_spacings(0);
   nameTable->set_col_spacings(0);
   nameTable->attach(*nameLabel, 0, 1, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   nameTable->attach(*nameEntry, 1, 2, 0, 1, Gtk::EXPAND|Gtk::FILL, Gtk::AttachOptions(), 4, 4);
   filterLabel->set_alignment(0.5,0.5);
   filterLabel->set_padding(4,4);
   filterLabel->set_justify(Gtk::JUSTIFY_LEFT);
   filterLabel->set_line_wrap(false);
   filterLabel->set_use_markup(false);
   filterLabel->set_selectable(false);
   addFilterButton->set_flags(Gtk::CAN_FOCUS);
   addFilterButton->set_relief(Gtk::RELIEF_NORMAL);
   queryHbox->pack_start(*filterLabel, Gtk::PACK_SHRINK, 0);
   queryHbox->pack_start(*filterCombobox, Gtk::PACK_EXPAND_WIDGET, 4);
   queryHbox->pack_start(*addFilterButton, Gtk::PACK_SHRINK, 4);
   queryTextview->set_flags(Gtk::CAN_FOCUS);
   queryTextview->set_editable(true);
   queryTextview->set_cursor_visible(true);
   queryTextview->set_pixels_above_lines(0);
   queryTextview->set_pixels_below_lines(0);
   queryTextview->set_pixels_inside_wrap(0);
   queryTextview->set_left_margin(0);
   queryTextview->set_right_margin(0);
   queryTextview->set_indent(0);
   queryTextview->set_wrap_mode(Gtk::WRAP_WORD);
   queryTextview->set_justification(Gtk::JUSTIFY_LEFT);
   queryScrolledwindow->set_flags(Gtk::CAN_FOCUS);
   queryScrolledwindow->set_shadow_type(Gtk::SHADOW_IN);
   queryScrolledwindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   queryScrolledwindow->property_window_placement().set_value(Gtk::CORNER_TOP_LEFT);
   queryScrolledwindow->add(*queryTextview);
   queryFrameAlignment->add(*queryScrolledwindow);
   queryFrameLabel->set_alignment(0.5,0.5);
   queryFrameLabel->set_padding(0,0);
   queryFrameLabel->set_justify(Gtk::JUSTIFY_LEFT);
   queryFrameLabel->set_line_wrap(false);
   queryFrameLabel->set_use_markup(true);
   queryFrameLabel->set_selectable(false);
   queryFrame->set_shadow_type(Gtk::SHADOW_NONE);
   queryFrame->set_label_align(0,0.5);
   queryFrame->add(*queryFrameAlignment);
   queryFrame->set_label_widget(*queryFrameLabel);
   indexCheckbutton->set_flags(Gtk::CAN_FOCUS);
   indexCheckbutton->set_relief(Gtk::RELIEF_NORMAL);
   indexCheckbutton->set_mode(true);
   indexCheckbutton->set_active(false);
   resultsCountLabel->set_alignment(0,0.5);
   resultsCountLabel->set_padding(0,0);
   resultsCountLabel->set_justify(Gtk::JUSTIFY_LEFT);
   resultsCountLabel->set_line_wrap(false);
   resultsCountLabel->set_use_markup(false);
   resultsCountLabel->set_selectable(false);
   resultsCountSpinbutton->set_flags(Gtk::CAN_FOCUS);
   resultsCountSpinbutton->set_update_policy(Gtk::UPDATE_ALWAYS);
   resultsCountSpinbutton->set_numeric(false);
   resultsCountSpinbutton->set_digits(0);
   resultsCountSpinbutton->set_wrap(false);
   fromCheckbutton->set_flags(Gtk::CAN_FOCUS);
   fromCheckbutton->set_relief(Gtk::RELIEF_NORMAL);
   fromCheckbutton->set_mode(true);
   fromCheckbutton->set_active(false);
   toCheckbutton->set_flags(Gtk::CAN_FOCUS);
   toCheckbutton->set_relief(Gtk::RELIEF_NORMAL);
   toCheckbutton->set_mode(true);
   toCheckbutton->set_active(false);
   fromButton->set_flags(Gtk::CAN_FOCUS);
   fromButton->set_relief(Gtk::RELIEF_NORMAL);
   toButton->set_flags(Gtk::CAN_FOCUS);
   toButton->set_relief(Gtk::RELIEF_NORMAL);
   actionsTable->set_row_spacings(0);
   actionsTable->set_col_spacings(0);
   actionsTable->attach(*indexCheckbutton, 0, 1, 3, 4, Gtk::FILL, Gtk::FILL, 4, 4);
   actionsTable->attach(*labelNameCombobox, 1, 2, 3, 4, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   actionsTable->attach(*resultsCountLabel, 0, 1, 0, 1, Gtk::FILL, Gtk::FILL, 4, 4);
   actionsTable->attach(*resultsCountSpinbutton, 1, 2, 0, 1, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   actionsTable->attach(*fromCheckbutton, 0, 1, 1, 2, Gtk::FILL, Gtk::FILL, 4, 4);
   actionsTable->attach(*toCheckbutton, 0, 1, 2, 3, Gtk::FILL, Gtk::FILL, 4, 4);
   actionsTable->attach(*fromButton, 1, 2, 1, 2, Gtk::FILL, Gtk::FILL, 4, 4);
   actionsTable->attach(*toButton, 1, 2, 2, 3, Gtk::FILL, Gtk::FILL, 4, 4);
   queryVbox->pack_start(*nameTable, Gtk::PACK_SHRINK, 0);
   queryVbox->pack_start(*queryHbox);
   queryVbox->pack_start(*queryFrame, Gtk::PACK_EXPAND_WIDGET, 4);
   queryVbox->pack_start(*actionsTable);
   queryDialog->get_vbox()->set_homogeneous(false);
   queryDialog->get_vbox()->set_spacing(0);
   queryDialog->get_vbox()->pack_start(*queryVbox);
   queryDialog->set_title(_("Query parameters"));
   queryDialog->set_modal(false);
   queryDialog->property_window_position().set_value(Gtk::WIN_POS_NONE);
   queryDialog->set_resizable(true);
   queryDialog->property_destroy_with_parent().set_value(false);
   queryDialog->set_has_separator(true);
   queryDialog->add_action_widget(*queryCancelbutton, -6);
   queryDialog->add_action_widget(*queryOkbutton, -5);
   queryCancelbutton->show();
   queryOkbutton->show();
   nameLabel->show();
   nameEntry->show();
   nameTable->show();
   filterLabel->show();
   filterCombobox->show();
   addFilterButton->show();
   queryHbox->show();
   queryTextview->show();
   queryScrolledwindow->show();
   queryFrameAlignment->show();
   queryFrameLabel->show();
   queryFrame->show();
   indexCheckbutton->show();
   labelNameCombobox->show();
   resultsCountLabel->show();
   resultsCountSpinbutton->show();
   fromCheckbutton->show();
   toCheckbutton->show();
   fromButton->show();
   toButton->show();
   actionsTable->show();
   queryVbox->show();
   queryDialog->show();
   queryOkbutton->signal_clicked().connect(SigC::slot(*this, &queryDialog_glade::on_queryOkbutton_clicked), false);
   nameEntry->signal_changed().connect(SigC::slot(*this, &queryDialog_glade::on_nameEntry_changed), false);
   addFilterButton->signal_clicked().connect(SigC::slot(*this, &queryDialog_glade::on_addFilterButton_clicked), false);
   fromButton->signal_clicked().connect(SigC::slot(*this, &queryDialog_glade::on_fromButton_clicked), false);
   toButton->signal_clicked().connect(SigC::slot(*this, &queryDialog_glade::on_toButton_clicked), false);
}

queryDialog_glade::~queryDialog_glade()
{  delete gmm_data;
}
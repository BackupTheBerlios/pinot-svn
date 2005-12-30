// generated 2005/11/6 16:43:41 SGT by fabrice@amra.dyndns.org.(none)
// using glademm V2.6.0
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/fabrice/Projects/MetaSE/metase/UI/GTK2/metase-gtk2.glade
// for gtk 2.6.10 and gtkmm 2.6.2
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
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif
#include <gtkmmconfig.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#include <sigc++/compatibility.h>
#define GMM_GTKMM_22_24(a,b) b
#else //gtkmm 2.2
#define GMM_GTKMM_22_24(a,b) a
#endif //
#include "importDialog_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/alignment.h>
#include <gtkmm/adjustment.h>

importDialog_glade::importDialog_glade(
)
{  importDialog = this;
   gmm_data = new GlademmData(get_accel_group());
   
   Gtk::Button *cancelbutton1 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
   importOkButton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   
   Gtk::Image *image180 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-open"), Gtk::IconSize(4)));
   Gtk::Label *label17 = Gtk::manage(new class Gtk::Label(_("Select")));
   Gtk::HBox *hbox14 = Gtk::manage(new class Gtk::HBox(false, 2));
   Gtk::Alignment *alignment9 = Gtk::manage(new class Gtk::Alignment(0.5, 0.5, 0, 0));
   selectButton = Gtk::manage(new class Gtk::Button());
   
   Gtk::HButtonBox *hbuttonbox3 = Gtk::manage(new class Gtk::HButtonBox(Gtk::BUTTONBOX_DEFAULT_STYLE, 0));
   locationEntry = Gtk::manage(new class Gtk::Entry());
   titleEntry = Gtk::manage(new class Gtk::Entry());
   
   Gtk::Label *locationLabel = Gtk::manage(new class Gtk::Label(_("Location:")));
   titleLabel = Gtk::manage(new class Gtk::Label(_("Title:")));
   depthLabel = Gtk::manage(new class Gtk::Label(_("Maximum depth:")));
   
   Gtk::Adjustment *depthSpinbutton_adj = Gtk::manage(new class Gtk::Adjustment(0, 0, 100, 1, 5, 5));
   depthSpinbutton = Gtk::manage(new class Gtk::SpinButton(*depthSpinbutton_adj, 1, 0));
   typeCombobox = Gtk::manage(new class Gtk::ComboBox());
   
   Gtk::Label *typeLabel = Gtk::manage(new class Gtk::Label(_("Type:")));
   docTable = Gtk::manage(new class Gtk::Table(2, 2, false));
   cancelbutton1->set_flags(Gtk::CAN_FOCUS);
   cancelbutton1->set_flags(Gtk::CAN_DEFAULT);
   cancelbutton1->set_relief(Gtk::RELIEF_NORMAL);
   importOkButton->set_flags(Gtk::CAN_FOCUS);
   importOkButton->set_flags(Gtk::CAN_DEFAULT);
   importOkButton->set_relief(Gtk::RELIEF_NORMAL);
   importDialog->get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
   image180->set_alignment(0.5,0.5);
   image180->set_padding(0,0);
   label17->set_alignment(0.5,0.5);
   label17->set_padding(0,0);
   label17->set_justify(Gtk::JUSTIFY_LEFT);
   label17->set_line_wrap(false);
   label17->set_use_markup(false);
   label17->set_selectable(false);
   hbox14->pack_start(*image180, Gtk::PACK_SHRINK, 0);
   hbox14->pack_start(*label17, Gtk::PACK_SHRINK, 0);
   alignment9->add(*hbox14);
   selectButton->set_flags(Gtk::CAN_FOCUS);
   selectButton->set_flags(Gtk::CAN_DEFAULT);
   selectButton->set_relief(Gtk::RELIEF_NORMAL);
   selectButton->add(*alignment9);
   hbuttonbox3->pack_start(*selectButton);
   locationEntry->set_flags(Gtk::CAN_FOCUS);
   locationEntry->set_visibility(true);
   locationEntry->set_editable(true);
   locationEntry->set_max_length(0);
   locationEntry->set_has_frame(true);
   locationEntry->set_activates_default(false);
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
   depthLabel->set_alignment(0,0.5);
   depthLabel->set_padding(4,4);
   depthLabel->set_justify(Gtk::JUSTIFY_LEFT);
   depthLabel->set_line_wrap(false);
   depthLabel->set_use_markup(false);
   depthLabel->set_selectable(false);
   depthSpinbutton->set_flags(Gtk::CAN_FOCUS);
   depthSpinbutton->set_update_policy(Gtk::UPDATE_ALWAYS);
   depthSpinbutton->set_numeric(false);
   depthSpinbutton->set_digits(0);
   depthSpinbutton->set_wrap(false);
   typeLabel->set_alignment(0,0.5);
   typeLabel->set_padding(4,4);
   typeLabel->set_justify(Gtk::JUSTIFY_LEFT);
   typeLabel->set_line_wrap(false);
   typeLabel->set_use_markup(false);
   typeLabel->set_selectable(false);
   docTable->set_row_spacings(0);
   docTable->set_col_spacings(0);
   docTable->attach(*hbuttonbox3, 2, 3, 3, 4, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   docTable->attach(*locationEntry, 1, 2, 3, 4, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   docTable->attach(*titleEntry, 1, 2, 1, 2, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   docTable->attach(*locationLabel, 0, 1, 3, 4, Gtk::FILL, Gtk::FILL, 0, 0);
   docTable->attach(*titleLabel, 0, 1, 1, 2, Gtk::FILL, Gtk::FILL, 0, 0);
   docTable->attach(*depthLabel, 0, 1, 2, 3, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   docTable->attach(*depthSpinbutton, 1, 2, 2, 3, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   docTable->attach(*typeCombobox, 1, 2, 0, 1, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   docTable->attach(*typeLabel, 0, 1, 0, 1, Gtk::FILL, Gtk::FILL, 0, 0);
   importDialog->get_vbox()->set_homogeneous(false);
   importDialog->get_vbox()->set_spacing(0);
   importDialog->get_vbox()->pack_start(*docTable);
   importDialog->set_title(_("Import document"));
   importDialog->set_modal(false);
   importDialog->property_window_position().set_value(Gtk::WIN_POS_NONE);
   importDialog->set_resizable(true);
   importDialog->property_destroy_with_parent().set_value(false);
   importDialog->set_has_separator(true);
   importDialog->add_action_widget(*cancelbutton1, -6);
   importDialog->add_action_widget(*importOkButton, -5);
   cancelbutton1->show();
   importOkButton->show();
   image180->show();
   label17->show();
   hbox14->show();
   alignment9->show();
   selectButton->show();
   hbuttonbox3->show();
   locationEntry->show();
   titleEntry->show();
   locationLabel->show();
   titleLabel->show();
   depthLabel->show();
   depthSpinbutton->show();
   typeCombobox->show();
   typeLabel->show();
   docTable->show();
   importDialog->show();
   importOkButton->signal_clicked().connect(SigC::slot(*this, &importDialog_glade::on_importOkButton_clicked), false);
   selectButton->signal_clicked().connect(SigC::slot(*this, &importDialog_glade::on_selectButton_clicked), false);
   locationEntry->signal_changed().connect(SigC::slot(*this, &importDialog_glade::on_locationEntry_changed), false);
   typeCombobox->signal_changed().connect(SigC::slot(*this, &importDialog_glade::on_typeCombobox_changed), false);
}

importDialog_glade::~importDialog_glade()
{  delete gmm_data;
}

// generated 2007/1/9 20:01:15 SGT by fabrice@amra.dyndns.org.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/fabrice/Projects/MetaSE/pinot/UI/GTK2/metase-gtk2.glade
// for gtk 2.10.4 and gtkmm 2.10.5
//
// Please modify the corresponding derived classes in ./src/statisticsDialog.cc


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
#include "statisticsDialog_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
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


statisticsDialog_glade::statisticsDialog_glade(
)
{  
   
   Gtk::Dialog *statisticsDialog = this;
   gmm_data = new GlademmData(get_accel_group());
   
   Gtk::Button *closebutton1 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-close")));
   statisticsTreeview = Gtk::manage(new class Gtk::TreeView());
   statisticsScrolledwindow = Gtk::manage(new class Gtk::ScrolledWindow());
   closebutton1->set_flags(Gtk::CAN_FOCUS);
   closebutton1->set_flags(Gtk::CAN_DEFAULT);
   closebutton1->set_relief(Gtk::RELIEF_NORMAL);
   statisticsDialog->get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
   statisticsTreeview->set_flags(Gtk::CAN_FOCUS);
   statisticsTreeview->set_headers_visible(false);
   statisticsTreeview->set_rules_hint(false);
   statisticsTreeview->set_reorderable(false);
   statisticsTreeview->set_enable_search(true);
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=5
   statisticsTreeview->set_fixed_height_mode(false);
   statisticsTreeview->set_hover_selection(false);
   statisticsTreeview->set_hover_expand(false);
#endif //
   statisticsScrolledwindow->set_flags(Gtk::CAN_FOCUS);
   statisticsScrolledwindow->set_border_width(4);
   statisticsScrolledwindow->set_shadow_type(Gtk::SHADOW_NONE);
   statisticsScrolledwindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   statisticsScrolledwindow->property_window_placement().set_value(Gtk::CORNER_TOP_LEFT);
   statisticsScrolledwindow->add(*statisticsTreeview);
   statisticsDialog->get_vbox()->set_homogeneous(false);
   statisticsDialog->get_vbox()->set_spacing(0);
   statisticsDialog->get_vbox()->pack_start(*statisticsScrolledwindow);
   statisticsDialog->set_title(_("Status"));
   statisticsDialog->set_default_size(150,350);
   statisticsDialog->set_modal(false);
   statisticsDialog->property_window_position().set_value(Gtk::WIN_POS_NONE);
   statisticsDialog->set_resizable(true);
   statisticsDialog->property_destroy_with_parent().set_value(false);
   statisticsDialog->set_has_separator(true);
   statisticsDialog->add_action_widget(*closebutton1, -7);
   closebutton1->show();
   statisticsTreeview->show();
   statisticsScrolledwindow->show();
   statisticsDialog->show();
}

statisticsDialog_glade::~statisticsDialog_glade()
{  delete gmm_data;
}

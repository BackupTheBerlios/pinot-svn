// generated 2009/1/2 21:12:23 SGT by fabrice@rexor.dyndns.org.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/fabrice/Projects/MetaSE/pinot/UI/GTK2/metase-gtk2.glade
// for gtk 2.14.5 and gtkmm 2.14.3
//
// Please modify the corresponding derived classes in ./src/mainWindow.cc


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
#include "mainWindow_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtk/gtkimagemenuitem.h>
#include <gtkmm/menuitem.h>
#include <gtkmm/image.h>
#include <gtkmm/menu.h>
#include <gtkmm/imagemenuitem.h>
#include <gtkmm/menubar.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/scrolledwindow.h>
#ifndef ENABLE_NLS
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif


mainWindow_glade::mainWindow_glade(
) : Gtk::Window(Gtk::WINDOW_TOPLEVEL)
{  mainWindow = this;
   gmm_data = new GlademmData(get_accel_group());
   open1 = NULL;
   
   Gtk::MenuItem *separator7 = NULL;
   opencache1 = NULL;
   Gtk::Image *image1007 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-go-up"), Gtk::IconSize(1)));
   openparent1 = NULL;
   Gtk::MenuItem *separator8 = NULL;
   Gtk::Image *image1008 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-add"), Gtk::IconSize(1)));
   addtoindex1 = NULL;
   Gtk::Image *image1009 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-refresh"), Gtk::IconSize(1)));
   updateindex1 = NULL;
   Gtk::Image *image1010 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-delete"), Gtk::IconSize(1)));
   unindex1 = NULL;
   Gtk::MenuItem *separator9 = NULL;
   morelikethis1 = NULL;
   searchthisfor1 = NULL;
   Gtk::MenuItem *separator10 = NULL;
   properties1 = NULL;
   separatormenuitem1 = NULL;
   quit1 = NULL;
   Gtk::Menu *fileMenuitem_menu = Gtk::manage(new class Gtk::Menu());
   fileMenuitem = NULL;
   cut1 = NULL;
   copy1 = NULL;
   paste1 = NULL;
   delete1 = NULL;
   Gtk::MenuItem *separator5 = NULL;
   preferences1 = NULL;
   Gtk::Menu *editMenuitem_menu = Gtk::manage(new class Gtk::Menu());
   editMenuitem = NULL;
   listcontents1 = NULL;
   Gtk::RadioMenuItem::Group _RadioMIGroup_searchenginegroup1;
   searchenginegroup1 = NULL;
   hostnamegroup1 = NULL;
   Gtk::Menu *groupresults1_menu = Gtk::manage(new class Gtk::Menu());
   groupresults1 = NULL;
   showextracts1 = NULL;
   Gtk::MenuItem *separator4 = NULL;
   Gtk::Image *image1011 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-open"), Gtk::IconSize(1)));
   import1 = NULL;
   Gtk::Image *image1012 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-save-as"), Gtk::IconSize(1)));
   export1 = NULL;
   Gtk::MenuItem *separator6 = NULL;
   Gtk::Image *image1013 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-info"), Gtk::IconSize(1)));
   Gtk::ImageMenuItem *status1 = NULL;
   Gtk::Menu *optionsMenuitem_menu = Gtk::manage(new class Gtk::Menu());
   Gtk::MenuItem *optionsMenuitem = NULL;
   about1 = NULL;
   Gtk::Menu *helpMenuitem_menu = Gtk::manage(new class Gtk::Menu());
   helpMenuitem = NULL;
   Gtk::MenuBar *mainMenubar = Gtk::manage(new class Gtk::MenuBar());
   enginesVbox = Gtk::manage(new class Gtk::VBox(false, 0));
   
   Gtk::Image *image439 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-add"), Gtk::IconSize(4)));
   addIndexButton = Gtk::manage(new class Gtk::Button());
   
   Gtk::Image *image438 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-remove"), Gtk::IconSize(4)));
   removeIndexButton = Gtk::manage(new class Gtk::Button());
   
   Gtk::HBox *indexHbox = Gtk::manage(new class Gtk::HBox(true, 4));
   leftVbox = Gtk::manage(new class Gtk::VBox(false, 0));
   
   Gtk::Image *image623 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-network"), Gtk::IconSize(4)));
   enginesTogglebutton = Gtk::manage(new class Gtk::ToggleButton());
   
   Gtk::Label *liveQueryLabel = Gtk::manage(new class Gtk::Label(_("Query:")));
   liveQueryEntry = Gtk::manage(new class Gtk::Entry());
   findButton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-find")));
   
   Gtk::VButtonBox *findVbuttonbox = Gtk::manage(new class Gtk::VButtonBox(Gtk::BUTTONBOX_DEFAULT_STYLE, 0));
   Gtk::HBox *liveQueryHbox = Gtk::manage(new class Gtk::HBox(false, 0));
   queryTreeview = Gtk::manage(new class Gtk::TreeView());
   
   Gtk::ScrolledWindow *queryScrolledwindow = Gtk::manage(new class Gtk::ScrolledWindow());
   addQueryButton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-add")));
   removeQueryButton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-remove")));
   queryHistoryButton = Gtk::manage(new class Gtk::Button(_("History")));
   findQueryButton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-find")));
   
   Gtk::VButtonBox *queryVbuttonbox = Gtk::manage(new class Gtk::VButtonBox(Gtk::BUTTONBOX_START, 0));
   Gtk::HBox *queryHbox = Gtk::manage(new class Gtk::HBox(false, 0));
   Gtk::Label *queryLabel = Gtk::manage(new class Gtk::Label(_("Stored queries")));
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   queryExpander = Gtk::manage(new class Gtk::Expander());
#else //
   queryExpander = Gtk::manage(new class Gtk::HandleBox());
#endif //
   rightVbox = Gtk::manage(new class Gtk::VBox(false, 0));
   mainHpaned = Gtk::manage(new class Gtk::HPaned());
   mainProgressbar = Gtk::manage(new class Gtk::ProgressBar());
   mainStatusbar = Gtk::manage(new class Gtk::Statusbar());
   
   Gtk::HBox *mainHbox = Gtk::manage(new class Gtk::HBox(false, 0));
   Gtk::VBox *mainVbox = Gtk::manage(new class Gtk::VBox(false, 0));
   
   fileMenuitem_menu->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-open")));
   open1 = (Gtk::ImageMenuItem *)&fileMenuitem_menu->items().back();
   
   fileMenuitem_menu->items().push_back(Gtk::Menu_Helpers::SeparatorElem());
   separator7 = (Gtk::MenuItem *)&fileMenuitem_menu->items().back();
   
   fileMenuitem_menu->items().push_back(Gtk::Menu_Helpers::MenuElem(_("Open Cache")));
   opencache1 = (Gtk::MenuItem *)&fileMenuitem_menu->items().back();
   
   fileMenuitem_menu->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Open Parent"), *image1007));
   openparent1 = (Gtk::ImageMenuItem *)&fileMenuitem_menu->items().back();
   
   fileMenuitem_menu->items().push_back(Gtk::Menu_Helpers::SeparatorElem());
   separator8 = (Gtk::MenuItem *)&fileMenuitem_menu->items().back();
   
   fileMenuitem_menu->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("_Add To Index"), *image1008));
   addtoindex1 = (Gtk::ImageMenuItem *)&fileMenuitem_menu->items().back();
   
   fileMenuitem_menu->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Update"), *image1009));
   updateindex1 = (Gtk::ImageMenuItem *)&fileMenuitem_menu->items().back();
   
   fileMenuitem_menu->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Unindex"), *image1010));
   unindex1 = (Gtk::ImageMenuItem *)&fileMenuitem_menu->items().back();
   
   fileMenuitem_menu->items().push_back(Gtk::Menu_Helpers::SeparatorElem());
   separator9 = (Gtk::MenuItem *)&fileMenuitem_menu->items().back();
   
   fileMenuitem_menu->items().push_back(Gtk::Menu_Helpers::MenuElem(_("More Like This")));
   morelikethis1 = (Gtk::MenuItem *)&fileMenuitem_menu->items().back();
   
   fileMenuitem_menu->items().push_back(Gtk::Menu_Helpers::MenuElem(_("Search This For")));
   searchthisfor1 = (Gtk::MenuItem *)&fileMenuitem_menu->items().back();
   
   fileMenuitem_menu->items().push_back(Gtk::Menu_Helpers::SeparatorElem());
   separator10 = (Gtk::MenuItem *)&fileMenuitem_menu->items().back();
   
   fileMenuitem_menu->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-properties")));
   properties1 = (Gtk::ImageMenuItem *)&fileMenuitem_menu->items().back();
   
   fileMenuitem_menu->items().push_back(Gtk::Menu_Helpers::SeparatorElem());
   separatormenuitem1 = (Gtk::MenuItem *)&fileMenuitem_menu->items().back();
   
   fileMenuitem_menu->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-quit")));
   quit1 = (Gtk::ImageMenuItem *)&fileMenuitem_menu->items().back();
   
   editMenuitem_menu->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-cut")));
   cut1 = (Gtk::ImageMenuItem *)&editMenuitem_menu->items().back();
   
   editMenuitem_menu->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-copy")));
   copy1 = (Gtk::ImageMenuItem *)&editMenuitem_menu->items().back();
   
   editMenuitem_menu->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-paste")));
   paste1 = (Gtk::ImageMenuItem *)&editMenuitem_menu->items().back();
   
   editMenuitem_menu->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-delete")));
   delete1 = (Gtk::ImageMenuItem *)&editMenuitem_menu->items().back();
   
   editMenuitem_menu->items().push_back(Gtk::Menu_Helpers::SeparatorElem());
   separator5 = (Gtk::MenuItem *)&editMenuitem_menu->items().back();
   
   editMenuitem_menu->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-preferences")));
   preferences1 = (Gtk::ImageMenuItem *)&editMenuitem_menu->items().back();
   
   groupresults1_menu->items().push_back(Gtk::Menu_Helpers::RadioMenuElem(_RadioMIGroup_searchenginegroup1, _("Search Engine")));
   searchenginegroup1 = (Gtk::RadioMenuItem *)&groupresults1_menu->items().back();
   
   groupresults1_menu->items().push_back(Gtk::Menu_Helpers::RadioMenuElem(_RadioMIGroup_searchenginegroup1, _("Host Name")));
   hostnamegroup1 = (Gtk::RadioMenuItem *)&groupresults1_menu->items().back();
   
   optionsMenuitem_menu->items().push_back(Gtk::Menu_Helpers::MenuElem(_("List Contents Of")));
   listcontents1 = (Gtk::MenuItem *)&optionsMenuitem_menu->items().back();
   
   optionsMenuitem_menu->items().push_back(Gtk::Menu_Helpers::MenuElem(_("Group Results By"), *groupresults1_menu));
   groupresults1 = (Gtk::MenuItem *)&optionsMenuitem_menu->items().back();
   
   optionsMenuitem_menu->items().push_back(Gtk::Menu_Helpers::CheckMenuElem(_("Show Extracts")));
   showextracts1 = (Gtk::CheckMenuItem *)&optionsMenuitem_menu->items().back();
   
   optionsMenuitem_menu->items().push_back(Gtk::Menu_Helpers::SeparatorElem());
   separator4 = (Gtk::MenuItem *)&optionsMenuitem_menu->items().back();
   
   optionsMenuitem_menu->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Import URL"), *image1011));
   import1 = (Gtk::ImageMenuItem *)&optionsMenuitem_menu->items().back();
   
   optionsMenuitem_menu->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Export List"), *image1012));
   export1 = (Gtk::ImageMenuItem *)&optionsMenuitem_menu->items().back();
   
   optionsMenuitem_menu->items().push_back(Gtk::Menu_Helpers::SeparatorElem());
   separator6 = (Gtk::MenuItem *)&optionsMenuitem_menu->items().back();
   
   optionsMenuitem_menu->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Status"), *image1013));
   status1 = (Gtk::ImageMenuItem *)&optionsMenuitem_menu->items().back();
   
   helpMenuitem_menu->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-about")));
   about1 = (Gtk::ImageMenuItem *)&helpMenuitem_menu->items().back();
   
   mainMenubar->items().push_back(Gtk::Menu_Helpers::MenuElem(_("_File"), *fileMenuitem_menu));
   fileMenuitem = (Gtk::MenuItem *)&mainMenubar->items().back();
   
   mainMenubar->items().push_back(Gtk::Menu_Helpers::MenuElem(_("_Edit"), *editMenuitem_menu));
   editMenuitem = (Gtk::MenuItem *)&mainMenubar->items().back();
   
   mainMenubar->items().push_back(Gtk::Menu_Helpers::MenuElem(_("_Options"), *optionsMenuitem_menu));
   optionsMenuitem = (Gtk::MenuItem *)&mainMenubar->items().back();
   
   mainMenubar->items().push_back(Gtk::Menu_Helpers::MenuElem(_("_Help"), *helpMenuitem_menu));
   helpMenuitem = (Gtk::MenuItem *)&mainMenubar->items().back();
   image1007->set_alignment(0.5,0.5);
   image1007->set_padding(0,0);
   image1008->set_alignment(0.5,0.5);
   image1008->set_padding(0,0);
   image1009->set_alignment(0.5,0.5);
   image1009->set_padding(0,0);
   image1010->set_alignment(0.5,0.5);
   image1010->set_padding(0,0);
   searchenginegroup1->set_active(true);
   hostnamegroup1->set_active(false);
   showextracts1->set_active(true);
   image1011->set_alignment(0.5,0.5);
   image1011->set_padding(0,0);
   image1012->set_alignment(0.5,0.5);
   image1012->set_padding(0,0);
   image1013->set_alignment(0.5,0.5);
   image1013->set_padding(0,0);
   image439->set_alignment(0.5,0.5);
   image439->set_padding(0,0);
   addIndexButton->set_flags(Gtk::CAN_FOCUS);
   addIndexButton->set_flags(Gtk::CAN_DEFAULT);
   addIndexButton->set_relief(Gtk::RELIEF_NORMAL);
   addIndexButton->add(*image439);
   image438->set_alignment(0.5,0.5);
   image438->set_padding(0,0);
   removeIndexButton->set_flags(Gtk::CAN_FOCUS);
   removeIndexButton->set_flags(Gtk::CAN_DEFAULT);
   removeIndexButton->set_relief(Gtk::RELIEF_NORMAL);
   removeIndexButton->add(*image438);
   indexHbox->pack_start(*addIndexButton);
   indexHbox->pack_start(*removeIndexButton);
   leftVbox->pack_start(*enginesVbox);
   leftVbox->pack_start(*indexHbox, Gtk::PACK_SHRINK, 0);
   image623->set_alignment(0.5,0.5);
   image623->set_padding(0,0);
   enginesTogglebutton->set_flags(Gtk::CAN_FOCUS);
   enginesTogglebutton->set_relief(Gtk::RELIEF_NORMAL);
   enginesTogglebutton->set_active(false);
   enginesTogglebutton->add(*image623);
   liveQueryLabel->set_alignment(0.5,0.5);
   liveQueryLabel->set_padding(0,0);
   liveQueryLabel->set_justify(Gtk::JUSTIFY_LEFT);
   liveQueryLabel->set_line_wrap(false);
   liveQueryLabel->set_use_markup(false);
   liveQueryLabel->set_selectable(false);
   liveQueryLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   liveQueryLabel->set_width_chars(-1);
   liveQueryLabel->set_angle(0);
   liveQueryLabel->set_single_line_mode(false);
   liveQueryEntry->set_flags(Gtk::CAN_FOCUS);
   liveQueryEntry->grab_focus();
   liveQueryEntry->set_visibility(true);
   liveQueryEntry->set_editable(true);
   liveQueryEntry->set_max_length(0);
   liveQueryEntry->set_has_frame(true);
   liveQueryEntry->set_activates_default(false);
   findButton->set_flags(Gtk::CAN_FOCUS);
   findButton->set_flags(Gtk::CAN_DEFAULT);
   findButton->set_relief(Gtk::RELIEF_NORMAL);
   findVbuttonbox->pack_start(*findButton);
   liveQueryHbox->pack_start(*enginesTogglebutton, Gtk::PACK_SHRINK, 4);
   liveQueryHbox->pack_start(*liveQueryLabel, Gtk::PACK_SHRINK, 4);
   liveQueryHbox->pack_start(*liveQueryEntry, Gtk::PACK_EXPAND_WIDGET, 4);
   liveQueryHbox->pack_start(*findVbuttonbox, Gtk::PACK_SHRINK, 4);
   queryTreeview->set_events(Gdk::BUTTON_PRESS_MASK);
   queryTreeview->set_flags(Gtk::CAN_FOCUS);
   queryTreeview->set_headers_visible(true);
   queryTreeview->set_rules_hint(false);
   queryTreeview->set_reorderable(false);
   queryTreeview->set_enable_search(false);
   queryTreeview->set_fixed_height_mode(false);
   queryTreeview->set_hover_selection(false);
   queryTreeview->set_hover_expand(false);
   queryScrolledwindow->set_flags(Gtk::CAN_FOCUS);
   queryScrolledwindow->set_shadow_type(Gtk::SHADOW_NONE);
   queryScrolledwindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   queryScrolledwindow->property_window_placement().set_value(Gtk::CORNER_TOP_LEFT);
   queryScrolledwindow->add(*queryTreeview);
   addQueryButton->set_flags(Gtk::CAN_FOCUS);
   addQueryButton->set_flags(Gtk::CAN_DEFAULT);
   addQueryButton->set_border_width(4);
   addQueryButton->set_relief(Gtk::RELIEF_NORMAL);
   removeQueryButton->set_flags(Gtk::CAN_FOCUS);
   removeQueryButton->set_flags(Gtk::CAN_DEFAULT);
   removeQueryButton->set_border_width(4);
   removeQueryButton->set_relief(Gtk::RELIEF_NORMAL);
   queryHistoryButton->set_flags(Gtk::CAN_FOCUS);
   queryHistoryButton->set_flags(Gtk::CAN_DEFAULT);
   queryHistoryButton->set_border_width(4);
   queryHistoryButton->set_relief(Gtk::RELIEF_NORMAL);
   findQueryButton->set_flags(Gtk::CAN_FOCUS);
   findQueryButton->set_flags(Gtk::CAN_DEFAULT);
   findQueryButton->set_border_width(4);
   findQueryButton->set_relief(Gtk::RELIEF_NORMAL);
   queryVbuttonbox->pack_start(*addQueryButton);
   queryVbuttonbox->pack_start(*removeQueryButton);
   queryVbuttonbox->pack_start(*queryHistoryButton);
   queryVbuttonbox->pack_start(*findQueryButton);
   queryHbox->pack_start(*queryScrolledwindow);
   queryHbox->pack_start(*queryVbuttonbox, Gtk::PACK_SHRINK, 0);
   queryLabel->set_alignment(0.5,0.5);
   queryLabel->set_padding(0,0);
   queryLabel->set_justify(Gtk::JUSTIFY_LEFT);
   queryLabel->set_line_wrap(false);
   queryLabel->set_use_markup(false);
   queryLabel->set_selectable(false);
   queryLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   queryLabel->set_width_chars(-1);
   queryLabel->set_angle(0);
   queryLabel->set_single_line_mode(false);
   queryExpander->set_flags(Gtk::CAN_FOCUS);
   queryExpander->set_expanded(false);
   queryExpander->set_spacing(0);
   queryExpander->add(*queryHbox);
   queryExpander->set_label_widget(*queryLabel);
   rightVbox->pack_start(*liveQueryHbox, Gtk::PACK_SHRINK, 0);
   rightVbox->pack_start(*queryExpander, Gtk::PACK_SHRINK, 0);
   mainHpaned->set_flags(Gtk::CAN_FOCUS);
   mainHpaned->set_position(200);
   mainHpaned->pack1(*leftVbox, Gtk::SHRINK);
   mainHpaned->pack2(*rightVbox, Gtk::EXPAND|Gtk::SHRINK);
   mainHbox->pack_start(*mainProgressbar, Gtk::PACK_SHRINK, 0);
   mainHbox->pack_start(*mainStatusbar);
   mainVbox->pack_start(*mainMenubar, Gtk::PACK_SHRINK, 0);
   mainVbox->pack_start(*mainHpaned, Gtk::PACK_EXPAND_WIDGET, 4);
   mainVbox->pack_start(*mainHbox, Gtk::PACK_SHRINK, 0);
   mainWindow->set_events(Gdk::BUTTON_PRESS_MASK);
   mainWindow->set_title(_("Pinot"));
   mainWindow->set_default_size(680,500);
   mainWindow->set_modal(false);
   mainWindow->property_window_position().set_value(Gtk::WIN_POS_NONE);
   mainWindow->set_resizable(true);
   mainWindow->property_destroy_with_parent().set_value(false);
   mainWindow->add(*mainVbox);
   open1->show();
   separator7->show();
   opencache1->show();
   image1007->show();
   openparent1->show();
   separator8->show();
   image1008->show();
   addtoindex1->show();
   image1009->show();
   updateindex1->show();
   image1010->show();
   unindex1->show();
   separator9->show();
   morelikethis1->show();
   searchthisfor1->show();
   separator10->show();
   properties1->show();
   separatormenuitem1->show();
   quit1->show();
   fileMenuitem->show();
   cut1->show();
   copy1->show();
   paste1->show();
   delete1->show();
   separator5->show();
   preferences1->show();
   editMenuitem->show();
   listcontents1->show();
   searchenginegroup1->show();
   hostnamegroup1->show();
   groupresults1->show();
   showextracts1->show();
   separator4->show();
   image1011->show();
   import1->show();
   image1012->show();
   export1->show();
   separator6->show();
   image1013->show();
   status1->show();
   optionsMenuitem->show();
   about1->show();
   helpMenuitem->show();
   mainMenubar->show();
   enginesVbox->show();
   image439->show();
   addIndexButton->show();
   image438->show();
   removeIndexButton->show();
   indexHbox->show();
   image623->show();
   enginesTogglebutton->show();
   liveQueryLabel->show();
   liveQueryEntry->show();
   findButton->show();
   liveQueryHbox->show();
   queryTreeview->show();
   queryScrolledwindow->show();
   addQueryButton->show();
   removeQueryButton->show();
   queryHistoryButton->show();
   findQueryButton->show();
   queryVbuttonbox->show();
   queryHbox->show();
   queryLabel->show();
   queryExpander->show();
   rightVbox->show();
   mainHpaned->show();
   mainProgressbar->show();
   mainStatusbar->show();
   mainHbox->show();
   mainVbox->show();
   open1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_open_activate), false);
   openparent1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_openparent_activate), false);
   addtoindex1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_addtoindex_activate), false);
   updateindex1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_updateindex_activate), false);
   unindex1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_unindex_activate), false);
   morelikethis1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_morelikethis_activate), false);
   properties1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_properties_activate), false);
   quit1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_quit_activate), false);
   cut1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_cut_activate), false);
   copy1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_copy_activate), false);
   paste1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_paste_activate), false);
   delete1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_delete_activate), false);
   preferences1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_preferences_activate), false);
   searchenginegroup1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_groupresults_activate), false);
   showextracts1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_showextracts_activate), false);
   import1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_import_activate), false);
   export1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_export_activate), false);
   status1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_status_activate), false);
   about1->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_about_activate), false);
   addIndexButton->signal_clicked().connect(sigc::mem_fun(*this, &mainWindow_glade::on_addIndexButton_clicked), false);
   removeIndexButton->signal_clicked().connect(sigc::mem_fun(*this, &mainWindow_glade::on_removeIndexButton_clicked), false);
   enginesTogglebutton->signal_toggled().connect(sigc::mem_fun(*this, &mainWindow_glade::on_enginesTogglebutton_toggled), false);
   liveQueryEntry->signal_changed().connect(sigc::mem_fun(*this, &mainWindow_glade::on_liveQueryEntry_changed), false);
   liveQueryEntry->signal_activate().connect(sigc::mem_fun(*this, &mainWindow_glade::on_liveQueryEntry_activate), false);
   findButton->signal_clicked().connect(sigc::mem_fun(*this, &mainWindow_glade::on_findButton_clicked), false);
   queryTreeview->signal_button_press_event().connect(sigc::mem_fun(*this, &mainWindow_glade::on_queryTreeview_button_press_event), false);
   addQueryButton->signal_clicked().connect(sigc::mem_fun(*this, &mainWindow_glade::on_addQueryButton_clicked), false);
   removeQueryButton->signal_clicked().connect(sigc::mem_fun(*this, &mainWindow_glade::on_removeQueryButton_clicked), false);
   queryHistoryButton->signal_clicked().connect(sigc::mem_fun(*this, &mainWindow_glade::on_queryHistoryButton_clicked), false);
   findQueryButton->signal_clicked().connect(sigc::mem_fun(*this, &mainWindow_glade::on_findQueryButton_clicked), false);
   mainWindow->signal_delete_event().connect(sigc::mem_fun(*this, &mainWindow_glade::on_mainWindow_delete_event), false);
}

mainWindow_glade::~mainWindow_glade()
{  delete gmm_data;
}

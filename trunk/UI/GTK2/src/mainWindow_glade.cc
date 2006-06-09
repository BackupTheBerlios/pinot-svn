// generated 2006/5/17 8:25:37 SGT by fabrice@amra.dyndns.org.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/fabrice/Projects/MetaSE/pinot/UI/GTK2/metase-gtk2.glade
// for gtk 2.8.17 and gtkmm 2.8.3
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
#include <sigc++/compatibility.h>
#include <sigc++/bind.h>
#define GMM_GTKMM_22_24(a,b) b
#else //gtkmm 2.2
#define GMM_GTKMM_22_24(a,b) a
#endif //
#include "mainWindow_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtk/gtkimagemenuitem.h>
#include <gtkmm/menu.h>
#include <gtkmm/image.h>
#include <gtkmm/menuitem.h>
#include <gtkmm/menubar.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/scrolledwindow.h>
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


mainWindow_glade::mainWindow_glade(
) : Gtk::Window(Gtk::WINDOW_TOPLEVEL)
{  mainWindow = this;
   gmm_data = new GlademmData(get_accel_group());
   configure1 = NULL;
   separatormenuitem1 = NULL;
   quit1 = NULL;
   
   Gtk::Menu *sessionMenuitem_menu = Gtk::manage(new class Gtk::Menu());
   sessionMenuitem = NULL;
   cut1 = NULL;
   copy1 = NULL;
   paste1 = NULL;
   delete1 = NULL;
   Gtk::Menu *editMenuitem_menu = Gtk::manage(new class Gtk::Menu());
   editMenuitem = NULL;
   showextract1 = NULL;
   Gtk::RadioMenuItem::Group _RadioMIGroup_searchenginegroup1;
   searchenginegroup1 = NULL;
   hostnamegroup1 = NULL;
   Gtk::Menu *groupresults1_menu = Gtk::manage(new class Gtk::Menu());
   groupresults1 = NULL;
   Gtk::Image *image584 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-clear"), Gtk::IconSize(1)));
   clearresults1 = NULL;
   Gtk::MenuItem *separator1 = NULL;
   viewresults1 = NULL;
   viewcache1 = NULL;
   indexresults1 = NULL;
   Gtk::Menu *resultsMenuitem_menu = Gtk::manage(new class Gtk::Menu());
   resultsMenuitem = NULL;
   list1 = NULL;
   Gtk::Image *image585 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-open"), Gtk::IconSize(1)));
   import1 = NULL;
   Gtk::MenuItem *separator3 = NULL;
   viewfromindex1 = NULL;
   Gtk::Image *image586 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-refresh"), Gtk::IconSize(1)));
   refreshindex1 = NULL;
   Gtk::Image *image587 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-delete"), Gtk::IconSize(1)));
   unindex1 = NULL;
   Gtk::Image *image588 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-properties"), Gtk::IconSize(1)));
   showfromindex1 = NULL;
   Gtk::Menu *indexMenuitem_menu = Gtk::manage(new class Gtk::Menu());
   indexMenuitem = NULL;
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
   Gtk::VBox *leftVbox = Gtk::manage(new class Gtk::VBox(false, 0));
   Gtk::Label *liveQueryLabel = Gtk::manage(new class Gtk::Label(_("Query:")));
   liveQueryEntry = Gtk::manage(new class Gtk::Entry());
   findButton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-find")));
   
   Gtk::VButtonBox *findVbuttonbox = Gtk::manage(new class Gtk::VButtonBox(Gtk::BUTTONBOX_DEFAULT_STYLE, 0));
   Gtk::HBox *liveQueryHbox = Gtk::manage(new class Gtk::HBox(false, 0));
   queryTreeview = Gtk::manage(new class Gtk::TreeView());
   
   Gtk::ScrolledWindow *queryScrolledwindow = Gtk::manage(new class Gtk::ScrolledWindow());
   addQueryButton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-add")));
   
   Gtk::Image *image400 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-open"), Gtk::IconSize(4)));
   Gtk::Label *label47 = Gtk::manage(new class Gtk::Label(_("Edit")));
   Gtk::HBox *hbox40 = Gtk::manage(new class Gtk::HBox(false, 2));
   Gtk::Alignment *alignment26 = Gtk::manage(new class Gtk::Alignment(0.5, 0.5, 0, 0));
   editQueryButton = Gtk::manage(new class Gtk::Button());
   removeQueryButton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-remove")));
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
   Gtk::VBox *vbox1 = Gtk::manage(new class Gtk::VBox(false, 0));
   
   sessionMenuitem_menu->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-preferences")));
   configure1 = (Gtk::ImageMenuItem *)&sessionMenuitem_menu->items().back();
   
   sessionMenuitem_menu->items().push_back(Gtk::Menu_Helpers::SeparatorElem());
   separatormenuitem1 = (Gtk::MenuItem *)&sessionMenuitem_menu->items().back();
   
   sessionMenuitem_menu->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-quit")));
   quit1 = (Gtk::ImageMenuItem *)&sessionMenuitem_menu->items().back();
   
   editMenuitem_menu->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-cut")));
   cut1 = (Gtk::ImageMenuItem *)&editMenuitem_menu->items().back();
   
   editMenuitem_menu->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-copy")));
   copy1 = (Gtk::ImageMenuItem *)&editMenuitem_menu->items().back();
   
   editMenuitem_menu->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-paste")));
   paste1 = (Gtk::ImageMenuItem *)&editMenuitem_menu->items().back();
   
   editMenuitem_menu->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-delete")));
   delete1 = (Gtk::ImageMenuItem *)&editMenuitem_menu->items().back();
   
   groupresults1_menu->items().push_back(Gtk::Menu_Helpers::RadioMenuElem(_RadioMIGroup_searchenginegroup1, _("Search Engine")));
   searchenginegroup1 = (Gtk::RadioMenuItem *)&groupresults1_menu->items().back();
   
   groupresults1_menu->items().push_back(Gtk::Menu_Helpers::RadioMenuElem(_RadioMIGroup_searchenginegroup1, _("Host Name")));
   hostnamegroup1 = (Gtk::RadioMenuItem *)&groupresults1_menu->items().back();
   
   resultsMenuitem_menu->items().push_back(Gtk::Menu_Helpers::CheckMenuElem(_("Show Extract")));
   showextract1 = (Gtk::CheckMenuItem *)&resultsMenuitem_menu->items().back();
   
   resultsMenuitem_menu->items().push_back(Gtk::Menu_Helpers::MenuElem(_("Group By"), *groupresults1_menu));
   groupresults1 = (Gtk::MenuItem *)&resultsMenuitem_menu->items().back();
   
   resultsMenuitem_menu->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Clear List"), *image584));
   clearresults1 = (Gtk::ImageMenuItem *)&resultsMenuitem_menu->items().back();
   
   resultsMenuitem_menu->items().push_back(Gtk::Menu_Helpers::SeparatorElem());
   separator1 = (Gtk::MenuItem *)&resultsMenuitem_menu->items().back();
   
   resultsMenuitem_menu->items().push_back(Gtk::Menu_Helpers::MenuElem(_("Vie_w")));
   viewresults1 = (Gtk::MenuItem *)&resultsMenuitem_menu->items().back();
   
   resultsMenuitem_menu->items().push_back(Gtk::Menu_Helpers::MenuElem(_("View Cache")));
   viewcache1 = (Gtk::MenuItem *)&resultsMenuitem_menu->items().back();
   
   resultsMenuitem_menu->items().push_back(Gtk::Menu_Helpers::MenuElem(_("_Index")));
   indexresults1 = (Gtk::MenuItem *)&resultsMenuitem_menu->items().back();
   
   indexMenuitem_menu->items().push_back(Gtk::Menu_Helpers::MenuElem(_("List Contents Of")));
   list1 = (Gtk::MenuItem *)&indexMenuitem_menu->items().back();
   
   indexMenuitem_menu->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Import"), *image585));
   import1 = (Gtk::ImageMenuItem *)&indexMenuitem_menu->items().back();
   
   indexMenuitem_menu->items().push_back(Gtk::Menu_Helpers::SeparatorElem());
   separator3 = (Gtk::MenuItem *)&indexMenuitem_menu->items().back();
   
   indexMenuitem_menu->items().push_back(Gtk::Menu_Helpers::MenuElem(_("View")));
   viewfromindex1 = (Gtk::MenuItem *)&indexMenuitem_menu->items().back();
   
   indexMenuitem_menu->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Update"), *image586));
   refreshindex1 = (Gtk::ImageMenuItem *)&indexMenuitem_menu->items().back();
   
   indexMenuitem_menu->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Unindex"), *image587));
   unindex1 = (Gtk::ImageMenuItem *)&indexMenuitem_menu->items().back();
   
   indexMenuitem_menu->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Properties"), *image588));
   showfromindex1 = (Gtk::ImageMenuItem *)&indexMenuitem_menu->items().back();
   
   helpMenuitem_menu->items().push_back(Gtk::Menu_Helpers::MenuElem(_("_About")));
   about1 = (Gtk::MenuItem *)&helpMenuitem_menu->items().back();
   
   mainMenubar->items().push_back(Gtk::Menu_Helpers::MenuElem(_("_Session"), *sessionMenuitem_menu));
   sessionMenuitem = (Gtk::MenuItem *)&mainMenubar->items().back();
   
   mainMenubar->items().push_back(Gtk::Menu_Helpers::MenuElem(_("_Edit"), *editMenuitem_menu));
   editMenuitem = (Gtk::MenuItem *)&mainMenubar->items().back();
   
   mainMenubar->items().push_back(Gtk::Menu_Helpers::MenuElem(_("_Results"), *resultsMenuitem_menu));
   resultsMenuitem = (Gtk::MenuItem *)&mainMenubar->items().back();
   
   mainMenubar->items().push_back(Gtk::Menu_Helpers::MenuElem(_("_Index"), *indexMenuitem_menu));
   indexMenuitem = (Gtk::MenuItem *)&mainMenubar->items().back();
   
   mainMenubar->items().push_back(Gtk::Menu_Helpers::MenuElem(_("_Help"), *helpMenuitem_menu));
   helpMenuitem = (Gtk::MenuItem *)&mainMenubar->items().back();
   showextract1->set_active(true);
   searchenginegroup1->set_active(true);
   hostnamegroup1->set_active(false);
   image584->set_alignment(0.5,0.5);
   image584->set_padding(0,0);
   image585->set_alignment(0.5,0.5);
   image585->set_padding(0,0);
   image586->set_alignment(0.5,0.5);
   image586->set_padding(0,0);
   image587->set_alignment(0.5,0.5);
   image587->set_padding(0,0);
   image588->set_alignment(0.5,0.5);
   image588->set_padding(0,0);
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
   liveQueryLabel->set_alignment(0.5,0.5);
   liveQueryLabel->set_padding(0,0);
   liveQueryLabel->set_justify(Gtk::JUSTIFY_LEFT);
   liveQueryLabel->set_line_wrap(false);
   liveQueryLabel->set_use_markup(false);
   liveQueryLabel->set_selectable(false);
   liveQueryEntry->set_flags(Gtk::CAN_FOCUS);
   liveQueryEntry->set_visibility(true);
   liveQueryEntry->set_editable(true);
   liveQueryEntry->set_max_length(0);
   liveQueryEntry->set_has_frame(true);
   liveQueryEntry->set_activates_default(false);
   findButton->set_flags(Gtk::CAN_FOCUS);
   findButton->set_flags(Gtk::CAN_DEFAULT);
   findButton->set_relief(Gtk::RELIEF_NORMAL);
   findVbuttonbox->pack_start(*findButton);
   liveQueryHbox->pack_start(*liveQueryLabel, Gtk::PACK_SHRINK, 4);
   liveQueryHbox->pack_start(*liveQueryEntry, Gtk::PACK_EXPAND_WIDGET, 4);
   liveQueryHbox->pack_start(*findVbuttonbox, Gtk::PACK_SHRINK, 4);
   queryTreeview->set_events(Gdk::BUTTON_PRESS_MASK);
   queryTreeview->set_flags(Gtk::CAN_FOCUS);
   queryTreeview->set_headers_visible(true);
   queryTreeview->set_rules_hint(false);
   queryTreeview->set_reorderable(false);
   queryTreeview->set_enable_search(false);
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=5
   queryTreeview->set_fixed_height_mode(false);
   queryTreeview->set_hover_selection(false);
   queryTreeview->set_hover_expand(false);
#endif //
   queryScrolledwindow->set_flags(Gtk::CAN_FOCUS);
   queryScrolledwindow->set_shadow_type(Gtk::SHADOW_NONE);
   queryScrolledwindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   queryScrolledwindow->property_window_placement().set_value(Gtk::CORNER_TOP_LEFT);
   queryScrolledwindow->add(*queryTreeview);
   addQueryButton->set_flags(Gtk::CAN_FOCUS);
   addQueryButton->set_flags(Gtk::CAN_DEFAULT);
   addQueryButton->set_border_width(4);
   addQueryButton->set_relief(Gtk::RELIEF_NORMAL);
   image400->set_alignment(0.5,0.5);
   image400->set_padding(0,0);
   label47->set_alignment(0.5,0.5);
   label47->set_padding(0,0);
   label47->set_justify(Gtk::JUSTIFY_LEFT);
   label47->set_line_wrap(false);
   label47->set_use_markup(false);
   label47->set_selectable(false);
   hbox40->pack_start(*image400, Gtk::PACK_SHRINK, 0);
   hbox40->pack_start(*label47, Gtk::PACK_SHRINK, 0);
   alignment26->add(*hbox40);
   editQueryButton->set_flags(Gtk::CAN_FOCUS);
   editQueryButton->set_flags(Gtk::CAN_DEFAULT);
   editQueryButton->set_border_width(4);
   editQueryButton->set_relief(Gtk::RELIEF_NORMAL);
   editQueryButton->add(*alignment26);
   removeQueryButton->set_flags(Gtk::CAN_FOCUS);
   removeQueryButton->set_flags(Gtk::CAN_DEFAULT);
   removeQueryButton->set_border_width(4);
   removeQueryButton->set_relief(Gtk::RELIEF_NORMAL);
   findQueryButton->set_flags(Gtk::CAN_FOCUS);
   findQueryButton->set_flags(Gtk::CAN_DEFAULT);
   findQueryButton->set_border_width(4);
   findQueryButton->set_relief(Gtk::RELIEF_NORMAL);
   queryVbuttonbox->pack_start(*addQueryButton);
   queryVbuttonbox->pack_start(*editQueryButton);
   queryVbuttonbox->pack_start(*removeQueryButton);
   queryVbuttonbox->pack_start(*findQueryButton);
   queryHbox->pack_start(*queryScrolledwindow);
   queryHbox->pack_start(*queryVbuttonbox, Gtk::PACK_SHRINK, 0);
   queryLabel->set_alignment(0.5,0.5);
   queryLabel->set_padding(0,0);
   queryLabel->set_justify(Gtk::JUSTIFY_LEFT);
   queryLabel->set_line_wrap(false);
   queryLabel->set_use_markup(false);
   queryLabel->set_selectable(false);
   queryExpander->set_flags(Gtk::CAN_FOCUS);
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   queryExpander->set_expanded(true);
   queryExpander->set_spacing(0);
#endif //
   queryExpander->add(*queryHbox);
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   queryExpander->set_label_widget(*queryLabel);
#endif //
   rightVbox->pack_start(*liveQueryHbox, Gtk::PACK_SHRINK, 0);
   rightVbox->pack_start(*queryExpander, Gtk::PACK_SHRINK, 0);
   mainHpaned->set_flags(Gtk::CAN_FOCUS);
   mainHpaned->set_position(200);
   mainHpaned->pack1(*leftVbox, Gtk::SHRINK);
   mainHpaned->pack2(*rightVbox, Gtk::EXPAND|Gtk::SHRINK);
   mainHbox->pack_start(*mainProgressbar, Gtk::PACK_SHRINK, 0);
   mainHbox->pack_start(*mainStatusbar);
   vbox1->pack_start(*mainMenubar, Gtk::PACK_SHRINK, 0);
   vbox1->pack_start(*mainHpaned, Gtk::PACK_EXPAND_WIDGET, 4);
   vbox1->pack_start(*mainHbox, Gtk::PACK_SHRINK, 0);
   mainWindow->set_events(Gdk::BUTTON_PRESS_MASK);
   mainWindow->set_title(_("Pinot"));
   mainWindow->set_default_size(680,500);
   mainWindow->set_modal(false);
   mainWindow->property_window_position().set_value(Gtk::WIN_POS_NONE);
   mainWindow->set_resizable(true);
   mainWindow->property_destroy_with_parent().set_value(false);
   mainWindow->add(*vbox1);
   configure1->show();
   separatormenuitem1->show();
   quit1->show();
   sessionMenuitem->show();
   cut1->show();
   copy1->show();
   paste1->show();
   delete1->show();
   editMenuitem->show();
   showextract1->show();
   searchenginegroup1->show();
   hostnamegroup1->show();
   groupresults1->show();
   image584->show();
   clearresults1->show();
   separator1->show();
   viewresults1->show();
   viewcache1->show();
   indexresults1->show();
   resultsMenuitem->show();
   list1->show();
   image585->show();
   import1->show();
   separator3->show();
   viewfromindex1->show();
   image586->show();
   refreshindex1->show();
   image587->show();
   unindex1->show();
   image588->show();
   showfromindex1->show();
   indexMenuitem->show();
   about1->show();
   helpMenuitem->show();
   mainMenubar->show();
   enginesVbox->show();
   image439->show();
   addIndexButton->show();
   image438->show();
   removeIndexButton->show();
   indexHbox->show();
   leftVbox->show();
   liveQueryLabel->show();
   liveQueryEntry->show();
   findButton->show();
   findVbuttonbox->show();
   liveQueryHbox->show();
   queryTreeview->show();
   queryScrolledwindow->show();
   addQueryButton->show();
   image400->show();
   label47->show();
   hbox40->show();
   alignment26->show();
   editQueryButton->show();
   removeQueryButton->show();
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
   vbox1->show();
   configure1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_configure_activate), false);
   quit1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_quit_activate), false);
   cut1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_cut_activate), false);
   copy1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_copy_activate), false);
   paste1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_paste_activate), false);
   delete1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_delete_activate), false);
   showextract1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_showextract_activate), false);
   searchenginegroup1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_groupresults_activate), false);
   clearresults1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_clearresults_activate), false);
   viewresults1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_viewresults_activate), false);
   viewcache1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_viewcache_activate), false);
   indexresults1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_indexresults_activate), false);
   import1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_import_activate), false);
   viewfromindex1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_viewfromindex_activate), false);
   refreshindex1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_refreshindex_activate), false);
   unindex1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_unindex_activate), false);
   showfromindex1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_showfromindex_activate), false);
   about1->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_about_activate), false);
   addIndexButton->signal_clicked().connect(SigC::slot(*this, &mainWindow_glade::on_addIndexButton_clicked), false);
   removeIndexButton->signal_clicked().connect(SigC::slot(*this, &mainWindow_glade::on_removeIndexButton_clicked), false);
   liveQueryEntry->signal_changed().connect(SigC::slot(*this, &mainWindow_glade::on_liveQueryEntry_changed), false);
   liveQueryEntry->signal_activate().connect(SigC::slot(*this, &mainWindow_glade::on_liveQueryEntry_activate), false);
   findButton->signal_clicked().connect(SigC::slot(*this, &mainWindow_glade::on_findButton_clicked), false);
   queryTreeview->signal_button_press_event().connect(SigC::slot(*this, &mainWindow_glade::on_queryTreeview_button_press_event), false);
   addQueryButton->signal_clicked().connect(SigC::slot(*this, &mainWindow_glade::on_addQueryButton_clicked), false);
   editQueryButton->signal_clicked().connect(SigC::slot(*this, &mainWindow_glade::on_editQueryButton_clicked), false);
   removeQueryButton->signal_clicked().connect(SigC::slot(*this, &mainWindow_glade::on_removeQueryButton_clicked), false);
   findQueryButton->signal_clicked().connect(SigC::slot(*this, &mainWindow_glade::on_findQueryButton_clicked), false);
   mainWindow->signal_delete_event().connect(SigC::slot(*this, &mainWindow_glade::on_mainWindow_delete_event), false);
}

mainWindow_glade::~mainWindow_glade()
{  delete gmm_data;
}

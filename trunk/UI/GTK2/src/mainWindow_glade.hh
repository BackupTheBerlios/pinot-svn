// generated 2007/2/17 18:49:42 SGT by fabrice@amra.dyndns.org.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/fabrice/Projects/MetaSE/pinot/UI/GTK2/metase-gtk2.glade
// for gtk 2.10.8 and gtkmm 2.10.7
//
// Please modify the corresponding derived classes in ./src/mainWindow.hh and./src/mainWindow.cc

#ifndef _MAINWINDOW_GLADE_HH
#  define _MAINWINDOW_GLADE_HH


#if !defined(GLADEMM_DATA)
#define GLADEMM_DATA 
#include <gtkmm/accelgroup.h>

class GlademmData
{  
        
        Glib::RefPtr<Gtk::AccelGroup> accgrp;
public:
        
        GlademmData(Glib::RefPtr<Gtk::AccelGroup> ag) : accgrp(ag)
        {  
        }
        
        Glib::RefPtr<Gtk::AccelGroup>  getAccelGroup()
        {  return accgrp;
        }
};
#endif //GLADEMM_DATA

#include <gtkmm/window.h>
#include <gtkmm/imagemenuitem.h>
#include <gtkmm/menuitem.h>
#include <gtkmm/checkmenuitem.h>
#include <gtkmm/radiomenuitem.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/entry.h>
#include <gtkmm/treeview.h>
#include <gtkmm/handlebox.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
#include <gtkmm/expander.h>
#else //
#endif //
#include <gtkmm/paned.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/statusbar.h>

class mainWindow_glade : public Gtk::Window
{  
        
        GlademmData *gmm_data;
public:
        class Gtk::Window * mainWindow;
protected:
        class Gtk::ImageMenuItem * configure1;
        class Gtk::MenuItem * separatormenuitem1;
        class Gtk::ImageMenuItem * quit1;
        class Gtk::MenuItem * sessionMenuitem;
        class Gtk::ImageMenuItem * cut1;
        class Gtk::ImageMenuItem * copy1;
        class Gtk::ImageMenuItem * paste1;
        class Gtk::ImageMenuItem * delete1;
        class Gtk::MenuItem * editMenuitem;
        class Gtk::CheckMenuItem * showextract1;
        class Gtk::RadioMenuItem * searchenginegroup1;
        class Gtk::RadioMenuItem * hostnamegroup1;
        class Gtk::MenuItem * groupresults1;
        class Gtk::ImageMenuItem * clearresults1;
        class Gtk::MenuItem * viewresults1;
        class Gtk::MenuItem * viewcache1;
        class Gtk::ImageMenuItem * morelikethis1;
        class Gtk::MenuItem * indexresults1;
        class Gtk::MenuItem * resultsMenuitem;
        class Gtk::MenuItem * list1;
        class Gtk::ImageMenuItem * import1;
        class Gtk::MenuItem * viewfromindex1;
        class Gtk::ImageMenuItem * refreshindex1;
        class Gtk::ImageMenuItem * unindex1;
        class Gtk::ImageMenuItem * showfromindex1;
        class Gtk::MenuItem * indexMenuitem;
        class Gtk::MenuItem * about1;
        class Gtk::MenuItem * helpMenuitem;
        class Gtk::VBox * enginesVbox;
        class Gtk::Button * addIndexButton;
        class Gtk::Button * removeIndexButton;
        class Gtk::VBox * leftVbox;
        class Gtk::ToggleButton * enginesTogglebutton;
        class Gtk::Entry * liveQueryEntry;
        class Gtk::Button * findButton;
        class Gtk::TreeView * queryTreeview;
        class Gtk::Button * addQueryButton;
        class Gtk::Button * editQueryButton;
        class Gtk::Button * removeQueryButton;
        class Gtk::Button * findQueryButton;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
        class Gtk::Expander * queryExpander;
#else //
        class Gtk::HandleBox * queryExpander;
#endif //
        class Gtk::VBox * rightVbox;
        class Gtk::HPaned * mainHpaned;
        class Gtk::ProgressBar * mainProgressbar;
        class Gtk::Statusbar * mainStatusbar;
        
        mainWindow_glade();
        
        ~mainWindow_glade();
private:
        virtual void on_configure_activate() = 0;
        virtual void on_statistics_activate() = 0;
        virtual void on_quit_activate() = 0;
        virtual void on_cut_activate() = 0;
        virtual void on_copy_activate() = 0;
        virtual void on_paste_activate() = 0;
        virtual void on_delete_activate() = 0;
        virtual void on_showextract_activate() = 0;
        virtual void on_groupresults_activate() = 0;
        virtual void on_clearresults_activate() = 0;
        virtual void on_viewresults_activate() = 0;
        virtual void on_morelikethis_activate() = 0;
        virtual void on_indexresults_activate() = 0;
        virtual void on_import_activate() = 0;
        virtual void on_viewfromindex_activate() = 0;
        virtual void on_refreshindex_activate() = 0;
        virtual void on_unindex_activate() = 0;
        virtual void on_showfromindex_activate() = 0;
        virtual void on_about_activate() = 0;
        virtual void on_addIndexButton_clicked() = 0;
        virtual void on_removeIndexButton_clicked() = 0;
        virtual void on_enginesTogglebutton_toggled() = 0;
        virtual void on_liveQueryEntry_changed() = 0;
        virtual void on_liveQueryEntry_activate() = 0;
        virtual void on_findButton_clicked() = 0;
        virtual bool on_queryTreeview_button_press_event(GdkEventButton *ev) = 0;
        virtual void on_addQueryButton_clicked() = 0;
        virtual void on_editQueryButton_clicked() = 0;
        virtual void on_removeQueryButton_clicked() = 0;
        virtual void on_findQueryButton_clicked() = 0;
        virtual bool on_mainWindow_delete_event(GdkEventAny *ev) = 0;
};
#endif

// generated 2009/1/2 21:12:23 SGT by fabrice@rexor.dyndns.org.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/fabrice/Projects/MetaSE/pinot/UI/GTK2/metase-gtk2.glade
// for gtk 2.14.5 and gtkmm 2.14.3
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
#include <gtkmm/radiomenuitem.h>
#include <gtkmm/checkmenuitem.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/entry.h>
#include <gtkmm/treeview.h>
#include <gtkmm/handlebox.h>
#include <gtkmm/expander.h>
#include <gtkmm/paned.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/statusbar.h>

class mainWindow_glade : public Gtk::Window
{  
        
        GlademmData *gmm_data;
public:
        class Gtk::Window * mainWindow;
protected:
        class Gtk::ImageMenuItem * open1;
        class Gtk::MenuItem * opencache1;
        class Gtk::ImageMenuItem * openparent1;
        class Gtk::ImageMenuItem * addtoindex1;
        class Gtk::ImageMenuItem * updateindex1;
        class Gtk::ImageMenuItem * unindex1;
        class Gtk::MenuItem * morelikethis1;
        class Gtk::MenuItem * searchthisfor1;
        class Gtk::ImageMenuItem * properties1;
        class Gtk::MenuItem * separatormenuitem1;
        class Gtk::ImageMenuItem * quit1;
        class Gtk::MenuItem * fileMenuitem;
        class Gtk::ImageMenuItem * cut1;
        class Gtk::ImageMenuItem * copy1;
        class Gtk::ImageMenuItem * paste1;
        class Gtk::ImageMenuItem * delete1;
        class Gtk::ImageMenuItem * preferences1;
        class Gtk::MenuItem * editMenuitem;
        class Gtk::MenuItem * listcontents1;
        class Gtk::RadioMenuItem * searchenginegroup1;
        class Gtk::RadioMenuItem * hostnamegroup1;
        class Gtk::MenuItem * groupresults1;
        class Gtk::CheckMenuItem * showextracts1;
        class Gtk::ImageMenuItem * import1;
        class Gtk::ImageMenuItem * export1;
        class Gtk::ImageMenuItem * about1;
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
        class Gtk::Button * removeQueryButton;
        class Gtk::Button * queryHistoryButton;
        class Gtk::Button * findQueryButton;
        class Gtk::Expander * queryExpander;
        class Gtk::VBox * rightVbox;
        class Gtk::HPaned * mainHpaned;
        class Gtk::ProgressBar * mainProgressbar;
        class Gtk::Statusbar * mainStatusbar;
        
        mainWindow_glade();
        
        ~mainWindow_glade();
private:
        virtual void on_open_activate() = 0;
        virtual void on_openparent_activate() = 0;
        virtual void on_addtoindex_activate() = 0;
        virtual void on_updateindex_activate() = 0;
        virtual void on_unindex_activate() = 0;
        virtual void on_morelikethis_activate() = 0;
        virtual void on_properties_activate() = 0;
        virtual void on_quit_activate() = 0;
        virtual void on_cut_activate() = 0;
        virtual void on_copy_activate() = 0;
        virtual void on_paste_activate() = 0;
        virtual void on_delete_activate() = 0;
        virtual void on_preferences_activate() = 0;
        virtual void on_groupresults_activate() = 0;
        virtual void on_showextracts_activate() = 0;
        virtual void on_import_activate() = 0;
        virtual void on_export_activate() = 0;
        virtual void on_status_activate() = 0;
        virtual void on_about_activate() = 0;
        virtual void on_addIndexButton_clicked() = 0;
        virtual void on_removeIndexButton_clicked() = 0;
        virtual void on_enginesTogglebutton_toggled() = 0;
        virtual void on_liveQueryEntry_changed() = 0;
        virtual void on_liveQueryEntry_activate() = 0;
        virtual void on_findButton_clicked() = 0;
        virtual bool on_queryTreeview_button_press_event(GdkEventButton *ev) = 0;
        virtual void on_addQueryButton_clicked() = 0;
        virtual void on_removeQueryButton_clicked() = 0;
        virtual void on_queryHistoryButton_clicked() = 0;
        virtual void on_findQueryButton_clicked() = 0;
        virtual bool on_mainWindow_delete_event(GdkEventAny *ev) = 0;
};
#endif

// generated 2006/5/5 15:18:57 SGT by fabrice@amra.dyndns.org.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- metase-gtk2.glade
// for gtk 2.8.17 and gtkmm 2.8.3
//
// Please modify the corresponding derived classes in ./src/propertiesDialog.hh and./src/propertiesDialog.cc

#ifndef _PROPERTIESDIALOG_GLADE_HH
#  define _PROPERTIESDIALOG_GLADE_HH


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

#include <gtkmm/dialog.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/combobox.h>
#include <gtkmm/treeview.h>
#include <gtkmm/scrolledwindow.h>

class propertiesDialog_glade : public Gtk::Dialog
{  
        
        GlademmData *gmm_data;
public:
        class Gtk::Dialog * propertiesDialog;
protected:
        class Gtk::Button * labelOkButton;
        class Gtk::Entry * titleEntry;
        class Gtk::Label * titleLabel;
        class Gtk::Label * languageLabel;
        class Gtk::Label * typeLabel;
        class Gtk::Entry * typeEntry;
        class Gtk::ComboBox * languageCombobox;
        class Gtk::TreeView * labelsTreeview;
        class Gtk::ScrolledWindow * labelsScrolledwindow;
        
        propertiesDialog_glade();
        
        ~propertiesDialog_glade();
private:
        virtual void on_labelOkButton_clicked() = 0;
};
#endif

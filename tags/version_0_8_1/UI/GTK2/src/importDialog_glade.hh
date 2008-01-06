// generated 2006/8/20 15:32:26 SGT by fabrice@amra.dyndns.org.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/fabrice/Projects/MetaSE/pinot/UI/GTK2/metase-gtk2.glade
// for gtk 2.8.20 and gtkmm 2.8.8
//
// Please modify the corresponding derived classes in ./src/importDialog.hh and./src/importDialog.cc

#ifndef _IMPORTDIALOG_GLADE_HH
#  define _IMPORTDIALOG_GLADE_HH


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
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/table.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/button.h>

class importDialog_glade : public Gtk::Dialog
{  
        
        GlademmData *gmm_data;
protected:
        class Gtk::Entry * titleEntry;
        class Gtk::Label * titleLabel;
        class Gtk::ComboBoxText * labelNameCombobox;
        class Gtk::Entry * locationEntry;
        class Gtk::Table * docTable;
        class Gtk::ProgressBar * importProgressbar;
        class Gtk::Button * importButton;
        
        importDialog_glade();
        
        ~importDialog_glade();
private:
        virtual void on_locationEntry_changed() = 0;
        virtual void on_importButton_clicked() = 0;
        virtual void on_importDialog_response(int response_id) = 0;
};
#endif
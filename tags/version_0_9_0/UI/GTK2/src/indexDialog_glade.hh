// generated 2006/12/29 9:49:18 SGT by fabrice@thorgrim.dyndns.org.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/fabrice/Projects/MetaSE/pinot/UI/GTK2/metase-gtk2.glade
// for gtk 2.10.4 and gtkmm 2.10.5
//
// Please modify the corresponding derived classes in ./src/indexDialog.hh and./src/indexDialog.cc

#ifndef _INDEXDIALOG_GLADE_HH
#  define _INDEXDIALOG_GLADE_HH


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
#include <gtkmm/spinbutton.h>
#include <gtkmm/comboboxtext.h>

class indexDialog_glade : public Gtk::Dialog
{  
        
        GlademmData *gmm_data;
protected:
        class Gtk::Button * indexCancelbutton;
        class Gtk::Button * indexOkbutton;
        class Gtk::Entry * locationEntry;
        class Gtk::Button * locationButton;
        class Gtk::Entry * nameEntry;
        class Gtk::SpinButton * portSpinbutton;
        class Gtk::ComboBoxText * typeCombobox;
        class Gtk::Entry * hostEntry;
        
        indexDialog_glade();
        
        ~indexDialog_glade();
private:
        virtual void on_indexOkbutton_clicked() = 0;
        virtual void on_locationEntry_changed() = 0;
        virtual void on_locationButton_clicked() = 0;
        virtual void on_nameEntry_changed() = 0;
        virtual void on_typeCombobox_changed() = 0;
};
#endif

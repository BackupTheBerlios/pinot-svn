// generated 2005/11/26 12:46:55 SGT by fabrice@amra.dyndns.org.(none)
// using glademm V2.6.0
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/fabrice/Projects/MetaSE/pinot/UI/GTK2/metase-gtk2.glade
// for gtk 2.6.10 and gtkmm 2.6.2
//
// Please modify the corresponding derived classes in ./src/queryDialog.hh and./src/queryDialog.cc

#ifndef _QUERYDIALOG_GLADE_HH
#  define _QUERYDIALOG_GLADE_HH


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
#include <gtkmm/checkbutton.h>
#include <gtkmm/combobox.h>
#include <gtkmm/notebook.h>

class queryDialog_glade : public Gtk::Dialog
{  
        
        GlademmData *gmm_data;
public:
        class Gtk::Dialog * queryDialog;
protected:
        class Gtk::Button * queryCancelbutton;
        class Gtk::Button * queryOkbutton;
        class Gtk::Entry * nameEntry;
        class Gtk::Entry * anyEntry;
        class Gtk::Entry * hostNameEntry;
        class Gtk::Entry * fileNameEntry;
        class Gtk::SpinButton * resultsCountSpinbutton;
        class Gtk::CheckButton * indexCheckbutton;
        class Gtk::ComboBox * labelCombobox;
        class Gtk::Entry * phraseEntry;
        class Gtk::Entry * notEntry;
        class Gtk::ComboBox * languageCombobox;
        class Gtk::Entry * andEntry;
        class Gtk::Notebook * queryNotebook;
        
        queryDialog_glade();
        
        ~queryDialog_glade();
private:
        virtual void on_queryOkbutton_clicked() = 0;
        virtual void on_nameEntry_changed() = 0;
};
#endif

// generated 2007/12/23 10:48:41 CET by fabrice@thorgrim.dyndns.org.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/fabrice/Projects/MetaSE/pinot/UI/GTK2/metase-gtk2.glade
// for gtk 2.12.3 and gtkmm 2.12.3
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
#include <gtkmm/comboboxtext.h>
#include <gtkmm/textview.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/spinbutton.h>

class queryDialog_glade : public Gtk::Dialog
{  
        
        GlademmData *gmm_data;
public:
        class Gtk::Dialog * queryDialog;
protected:
        class Gtk::Button * queryOkbutton;
        class Gtk::Entry * nameEntry;
        class Gtk::ComboBoxText * filterCombobox;
        class Gtk::TextView * queryTextview;
        class Gtk::CheckButton * indexCheckbutton;
        class Gtk::ComboBoxText * labelNameCombobox;
        class Gtk::SpinButton * resultsCountSpinbutton;
        class Gtk::ComboBoxText * sortOrderCombobox;
        class Gtk::ComboBoxText * stemmingCombobox;
        
        queryDialog_glade();
        
        ~queryDialog_glade();
private:
        virtual void on_queryOkbutton_clicked() = 0;
        virtual void on_nameEntry_changed() = 0;
        virtual void on_addFilterButton_clicked() = 0;
};
#endif

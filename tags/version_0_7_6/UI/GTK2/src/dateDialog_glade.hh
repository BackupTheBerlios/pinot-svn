// generated 2007/1/11 20:22:49 SGT by fabrice@amra.dyndns.org.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/fabrice/Projects/MetaSE/pinot/UI/GTK2/metase-gtk2.glade
// for gtk 2.10.4 and gtkmm 2.10.5
//
// Please modify the corresponding derived classes in ./src/dateDialog.hh and./src/dateDialog.cc

#ifndef _DATEDIALOG_GLADE_HH
#  define _DATEDIALOG_GLADE_HH


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
#include <gtkmm/calendar.h>

class dateDialog_glade : public Gtk::Dialog
{  
        
        GlademmData *gmm_data;
protected:
        class Gtk::Calendar * dateCalendar;
        
        dateDialog_glade();
        
        ~dateDialog_glade();
};
#endif

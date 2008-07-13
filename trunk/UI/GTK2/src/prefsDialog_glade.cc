// generated 2008/7/13 20:38:58 SGT by fabrice@rexor.dyndns.org.(none)
// using glademm V2.12.1
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/fabrice/Projects/MetaSE/pinot/UI/GTK2/metase-gtk2.glade
// for gtk 2.12.11 and gtkmm 2.12.7
//
// Please modify the corresponding derived classes in ./src/prefsDialog.cc


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
#include "prefsDialog_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/label.h>
#include <gtkmm/table.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/image.h>
#include <gtkmm/box.h>
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


prefsDialog_glade::prefsDialog_glade(
)
{  prefsDialog = this;
   gmm_data = new GlademmData(get_accel_group());
   prefsCancelbutton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
   prefsOkbutton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   apiKeyLabel = Gtk::manage(new class Gtk::Label(_("Google API key:")));
   apiKeyEntry = Gtk::manage(new class Gtk::Entry());
   
   Gtk::Label *newResultsLabel = Gtk::manage(new class Gtk::Label(_("New results:")));
   Gtk::Label *queriesLabel = Gtk::manage(new class Gtk::Label(_("Queries:")));
   enableCompletionCheckbutton = Gtk::manage(new class Gtk::CheckButton(_("Enable search terms suggestion")));
   newResultsColorbutton = Gtk::manage(new class Gtk::ColorButton());
   ignoreRobotsCheckbutton = Gtk::manage(new class Gtk::CheckButton(_("Ignore robots.txt and Robots META tag")));
   robotsLabel = Gtk::manage(new class Gtk::Label(_("HTTP crawling:")));
   
   generalTable = Gtk::manage(new class Gtk::Table(2, 2, false));
   Gtk::Label *generalLabel = Gtk::manage(new class Gtk::Label(_("General")));
   Gtk::RadioButton::Group _RadioBGroup_directConnectionRadiobutton;
   directConnectionRadiobutton = Gtk::manage(new class Gtk::RadioButton(_RadioBGroup_directConnectionRadiobutton, _("Direct connection to the Internet")));
   proxyRadiobutton = Gtk::manage(new class Gtk::RadioButton(_RadioBGroup_directConnectionRadiobutton, _("Manual proxy configuration:")));
   
   Gtk::Label *proxyAddressLabel = Gtk::manage(new class Gtk::Label(_("Address:")));
   proxyAddressEntry = Gtk::manage(new class Gtk::Entry());
   
   Gtk::Label *proxyTypeLabel = Gtk::manage(new class Gtk::Label(_("Type:")));
   Gtk::Label *proxyPortLabel = Gtk::manage(new class Gtk::Label(_("Port:")));
   Gtk::Adjustment *proxyPortSpinbutton_adj = Gtk::manage(new class Gtk::Adjustment(80, 1, 65535, 1, 10, 10));
   proxyPortSpinbutton = Gtk::manage(new class Gtk::SpinButton(*proxyPortSpinbutton_adj, 1, 0));
   proxyTypeCombobox = Gtk::manage(new class Gtk::ComboBoxText());
   
   Gtk::Table *socksTable = Gtk::manage(new class Gtk::Table(2, 2, false));
   Gtk::Table *netTable = Gtk::manage(new class Gtk::Table(2, 2, false));
   Gtk::Label *netLabel = Gtk::manage(new class Gtk::Label(_("Network")));
   Gtk::Label *indexLabelsLabel = Gtk::manage(new class Gtk::Label(_("Labels are used to classify indexed documents:")));
   labelsTreeview = Gtk::manage(new class Gtk::TreeView());
   
   Gtk::ScrolledWindow *labelsScrolledwindow = Gtk::manage(new class Gtk::ScrolledWindow());
   Gtk::Image *image235 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-add"), Gtk::IconSize(4)));
   Gtk::Label *label35 = Gtk::manage(new class Gtk::Label(_("Add")));
   Gtk::HBox *hbox29 = Gtk::manage(new class Gtk::HBox(false, 2));
   Gtk::Alignment *alignment17 = Gtk::manage(new class Gtk::Alignment(0.5, 0.5, 0, 0));
   addLabelButton = Gtk::manage(new class Gtk::Button());
   
   Gtk::Image *image236 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-remove"), Gtk::IconSize(4)));
   Gtk::Label *label36 = Gtk::manage(new class Gtk::Label(_("Remove")));
   Gtk::HBox *hbox30 = Gtk::manage(new class Gtk::HBox(false, 2));
   Gtk::Alignment *alignment18 = Gtk::manage(new class Gtk::Alignment(0.5, 0.5, 0, 0));
   removeLabelButton = Gtk::manage(new class Gtk::Button());
   
   Gtk::VButtonBox *labelsVbuttonbox = Gtk::manage(new class Gtk::VButtonBox(Gtk::BUTTONBOX_START, 0));
   Gtk::HBox *labelsHbox = Gtk::manage(new class Gtk::HBox(false, 0));
   Gtk::VBox *labelsVbox = Gtk::manage(new class Gtk::VBox(false, 0));
   Gtk::Label *labelsLabel = Gtk::manage(new class Gtk::Label(_("Labels")));
   Gtk::Label *directoriesLabel = Gtk::manage(new class Gtk::Label(_("These directories will be indexed and optionally monitored for changes:")));
   directoriesTreeview = Gtk::manage(new class Gtk::TreeView());
   
   Gtk::ScrolledWindow *directoriesScrolledwindow = Gtk::manage(new class Gtk::ScrolledWindow());
   Gtk::Image *image609 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-add"), Gtk::IconSize(4)));
   Gtk::Label *label58 = Gtk::manage(new class Gtk::Label(_("Add")));
   Gtk::HBox *hbox54 = Gtk::manage(new class Gtk::HBox(false, 2));
   Gtk::Alignment *alignment35 = Gtk::manage(new class Gtk::Alignment(0.5, 0.5, 0, 0));
   addDirectoryButton = Gtk::manage(new class Gtk::Button());
   removeDirectoryButton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-remove")));
   
   Gtk::VButtonBox *directoriesVbuttonbox = Gtk::manage(new class Gtk::VButtonBox(Gtk::BUTTONBOX_START, 0));
   Gtk::HBox *directoriesHbox = Gtk::manage(new class Gtk::HBox(false, 0));
   Gtk::Label *patternsLabel = Gtk::manage(new class Gtk::Label(_("File patterns:")));
   patternsTreeview = Gtk::manage(new class Gtk::TreeView());
   
   Gtk::ScrolledWindow *patternsScrolledwindow = Gtk::manage(new class Gtk::ScrolledWindow());
   patternsCombobox = Gtk::manage(new class Gtk::ComboBoxText());
   
   Gtk::VBox *patternsVbox = Gtk::manage(new class Gtk::VBox(false, 0));
   Gtk::Image *image624 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-add"), Gtk::IconSize(4)));
   Gtk::Label *label60 = Gtk::manage(new class Gtk::Label(_("Add")));
   Gtk::HBox *hbox56 = Gtk::manage(new class Gtk::HBox(false, 2));
   Gtk::Alignment *alignment36 = Gtk::manage(new class Gtk::Alignment(0.5, 0.5, 0, 0));
   addPatternButton = Gtk::manage(new class Gtk::Button());
   removePatternButton = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-remove")));
   resetPatternsButton = Gtk::manage(new class Gtk::Button(_("Reset")));
   
   Gtk::VButtonBox *patternsVbuttonbox = Gtk::manage(new class Gtk::VButtonBox(Gtk::BUTTONBOX_START, 0));
   Gtk::HBox *patternsHbox = Gtk::manage(new class Gtk::HBox(false, 0));
   Gtk::VBox *indexingVbox = Gtk::manage(new class Gtk::VBox(false, 0));
   Gtk::Label *indexingLabel = Gtk::manage(new class Gtk::Label(_("Indexing")));
   prefsNotebook = Gtk::manage(new class Gtk::Notebook());
   prefsCancelbutton->set_flags(Gtk::CAN_FOCUS);
   prefsCancelbutton->set_flags(Gtk::CAN_DEFAULT);
   prefsCancelbutton->set_relief(Gtk::RELIEF_NORMAL);
   prefsOkbutton->set_flags(Gtk::CAN_FOCUS);
   prefsOkbutton->set_flags(Gtk::CAN_DEFAULT);
   prefsOkbutton->set_relief(Gtk::RELIEF_NORMAL);
   prefsDialog->get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
   apiKeyLabel->set_alignment(0,0.5);
   apiKeyLabel->set_padding(4,4);
   apiKeyLabel->set_justify(Gtk::JUSTIFY_LEFT);
   apiKeyLabel->set_line_wrap(false);
   apiKeyLabel->set_use_markup(false);
   apiKeyLabel->set_selectable(false);
   apiKeyLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   apiKeyLabel->set_width_chars(-1);
   apiKeyLabel->set_angle(0);
   apiKeyLabel->set_single_line_mode(false);
   apiKeyEntry->set_flags(Gtk::CAN_FOCUS);
   apiKeyEntry->set_visibility(true);
   apiKeyEntry->set_editable(true);
   apiKeyEntry->set_max_length(0);
   apiKeyEntry->set_has_frame(true);
   apiKeyEntry->set_activates_default(false);
   newResultsLabel->set_alignment(0,0.5);
   newResultsLabel->set_padding(4,4);
   newResultsLabel->set_justify(Gtk::JUSTIFY_LEFT);
   newResultsLabel->set_line_wrap(false);
   newResultsLabel->set_use_markup(false);
   newResultsLabel->set_selectable(false);
   newResultsLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   newResultsLabel->set_width_chars(-1);
   newResultsLabel->set_angle(0);
   newResultsLabel->set_single_line_mode(false);
   queriesLabel->set_alignment(0,0.5);
   queriesLabel->set_padding(4,4);
   queriesLabel->set_justify(Gtk::JUSTIFY_LEFT);
   queriesLabel->set_line_wrap(false);
   queriesLabel->set_use_markup(false);
   queriesLabel->set_selectable(false);
   queriesLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   queriesLabel->set_width_chars(-1);
   queriesLabel->set_angle(0);
   queriesLabel->set_single_line_mode(false);
   enableCompletionCheckbutton->set_flags(Gtk::CAN_FOCUS);
   enableCompletionCheckbutton->set_relief(Gtk::RELIEF_NORMAL);
   enableCompletionCheckbutton->set_mode(true);
   enableCompletionCheckbutton->set_active(false);
   newResultsColorbutton->set_flags(Gtk::CAN_FOCUS);
   ignoreRobotsCheckbutton->set_flags(Gtk::CAN_FOCUS);
   ignoreRobotsCheckbutton->set_relief(Gtk::RELIEF_NORMAL);
   ignoreRobotsCheckbutton->set_mode(true);
   ignoreRobotsCheckbutton->set_active(false);
   robotsLabel->set_alignment(0,0.5);
   robotsLabel->set_padding(4,4);
   robotsLabel->set_justify(Gtk::JUSTIFY_LEFT);
   robotsLabel->set_line_wrap(false);
   robotsLabel->set_use_markup(false);
   robotsLabel->set_selectable(false);
   robotsLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   robotsLabel->set_width_chars(-1);
   robotsLabel->set_angle(0);
   robotsLabel->set_single_line_mode(false);
   generalTable->set_row_spacings(0);
   generalTable->set_col_spacings(0);
   generalTable->attach(*apiKeyLabel, 0, 1, 3, 4, Gtk::FILL, Gtk::FILL, 0, 0);
   generalTable->attach(*apiKeyEntry, 1, 2, 3, 4, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   generalTable->attach(*newResultsLabel, 0, 1, 1, 2, Gtk::FILL, Gtk::FILL, 0, 0);
   generalTable->attach(*queriesLabel, 0, 1, 2, 3, Gtk::FILL, Gtk::FILL, 0, 0);
   generalTable->attach(*enableCompletionCheckbutton, 1, 2, 2, 3, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   generalTable->attach(*newResultsColorbutton, 1, 2, 1, 2, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   generalTable->attach(*ignoreRobotsCheckbutton, 1, 2, 0, 1, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   generalTable->attach(*robotsLabel, 0, 1, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   generalLabel->set_alignment(0.5,0.5);
   generalLabel->set_padding(0,0);
   generalLabel->set_justify(Gtk::JUSTIFY_LEFT);
   generalLabel->set_line_wrap(false);
   generalLabel->set_use_markup(false);
   generalLabel->set_selectable(false);
   generalLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   generalLabel->set_width_chars(-1);
   generalLabel->set_angle(0);
   generalLabel->set_single_line_mode(false);
   directConnectionRadiobutton->set_flags(Gtk::CAN_FOCUS);
   directConnectionRadiobutton->set_relief(Gtk::RELIEF_NORMAL);
   directConnectionRadiobutton->set_mode(true);
   directConnectionRadiobutton->set_active(true);
   proxyRadiobutton->set_flags(Gtk::CAN_FOCUS);
   proxyRadiobutton->set_relief(Gtk::RELIEF_NORMAL);
   proxyRadiobutton->set_mode(true);
   proxyRadiobutton->set_active(false);
   proxyAddressLabel->set_alignment(0,0.5);
   proxyAddressLabel->set_padding(0,0);
   proxyAddressLabel->set_justify(Gtk::JUSTIFY_LEFT);
   proxyAddressLabel->set_line_wrap(false);
   proxyAddressLabel->set_use_markup(false);
   proxyAddressLabel->set_selectable(false);
   proxyAddressLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   proxyAddressLabel->set_width_chars(-1);
   proxyAddressLabel->set_angle(0);
   proxyAddressLabel->set_single_line_mode(false);
   proxyAddressEntry->set_flags(Gtk::CAN_FOCUS);
   proxyAddressEntry->set_visibility(true);
   proxyAddressEntry->set_editable(true);
   proxyAddressEntry->set_max_length(0);
   proxyAddressEntry->set_has_frame(true);
   proxyAddressEntry->set_activates_default(false);
   proxyTypeLabel->set_alignment(0,0.5);
   proxyTypeLabel->set_padding(0,0);
   proxyTypeLabel->set_justify(Gtk::JUSTIFY_LEFT);
   proxyTypeLabel->set_line_wrap(false);
   proxyTypeLabel->set_use_markup(false);
   proxyTypeLabel->set_selectable(false);
   proxyTypeLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   proxyTypeLabel->set_width_chars(-1);
   proxyTypeLabel->set_angle(0);
   proxyTypeLabel->set_single_line_mode(false);
   proxyPortLabel->set_alignment(0,0.5);
   proxyPortLabel->set_padding(0,0);
   proxyPortLabel->set_justify(Gtk::JUSTIFY_LEFT);
   proxyPortLabel->set_line_wrap(false);
   proxyPortLabel->set_use_markup(false);
   proxyPortLabel->set_selectable(false);
   proxyPortLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   proxyPortLabel->set_width_chars(-1);
   proxyPortLabel->set_angle(0);
   proxyPortLabel->set_single_line_mode(false);
   proxyPortSpinbutton->set_flags(Gtk::CAN_FOCUS);
   proxyPortSpinbutton->set_update_policy(Gtk::UPDATE_ALWAYS);
   proxyPortSpinbutton->set_numeric(false);
   proxyPortSpinbutton->set_digits(0);
   proxyPortSpinbutton->set_wrap(false);
   socksTable->set_row_spacings(0);
   socksTable->set_col_spacings(0);
   socksTable->attach(*proxyAddressLabel, 0, 1, 1, 2, Gtk::FILL, Gtk::FILL, 4, 4);
   socksTable->attach(*proxyAddressEntry, 1, 2, 1, 2, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   socksTable->attach(*proxyTypeLabel, 0, 1, 0, 1, Gtk::FILL, Gtk::FILL, 4, 4);
   socksTable->attach(*proxyPortLabel, 0, 1, 2, 3, Gtk::FILL, Gtk::FILL, 4, 4);
   socksTable->attach(*proxyPortSpinbutton, 1, 2, 2, 3, Gtk::EXPAND|Gtk::FILL, Gtk::AttachOptions(), 4, 4);
   socksTable->attach(*proxyTypeCombobox, 1, 2, 0, 1, Gtk::EXPAND|Gtk::FILL, Gtk::AttachOptions(), 4, 4);
   netTable->set_row_spacings(0);
   netTable->set_col_spacings(0);
   netTable->attach(*directConnectionRadiobutton, 0, 1, 0, 1, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   netTable->attach(*proxyRadiobutton, 0, 1, 1, 2, Gtk::EXPAND|Gtk::FILL, Gtk::FILL, 4, 4);
   netTable->attach(*socksTable, 0, 1, 2, 3, Gtk::FILL, Gtk::EXPAND|Gtk::FILL, 0, 0);
   netLabel->set_alignment(0.5,0.5);
   netLabel->set_padding(0,0);
   netLabel->set_justify(Gtk::JUSTIFY_LEFT);
   netLabel->set_line_wrap(false);
   netLabel->set_use_markup(false);
   netLabel->set_selectable(false);
   netLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   netLabel->set_width_chars(-1);
   netLabel->set_angle(0);
   netLabel->set_single_line_mode(false);
   indexLabelsLabel->set_alignment(0,0.5);
   indexLabelsLabel->set_padding(4,4);
   indexLabelsLabel->set_justify(Gtk::JUSTIFY_LEFT);
   indexLabelsLabel->set_line_wrap(true);
   indexLabelsLabel->set_use_markup(false);
   indexLabelsLabel->set_selectable(false);
   indexLabelsLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   indexLabelsLabel->set_width_chars(-1);
   indexLabelsLabel->set_angle(0);
   indexLabelsLabel->set_single_line_mode(false);
   labelsTreeview->set_flags(Gtk::CAN_FOCUS);
   labelsTreeview->set_headers_visible(true);
   labelsTreeview->set_rules_hint(false);
   labelsTreeview->set_reorderable(false);
   labelsTreeview->set_enable_search(true);
   labelsScrolledwindow->set_flags(Gtk::CAN_FOCUS);
   labelsScrolledwindow->set_border_width(4);
   labelsScrolledwindow->set_shadow_type(Gtk::SHADOW_NONE);
   labelsScrolledwindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   labelsScrolledwindow->property_window_placement().set_value(Gtk::CORNER_TOP_LEFT);
   labelsScrolledwindow->add(*labelsTreeview);
   image235->set_alignment(0.5,0.5);
   image235->set_padding(0,0);
   label35->set_alignment(0.5,0.5);
   label35->set_padding(0,0);
   label35->set_justify(Gtk::JUSTIFY_LEFT);
   label35->set_line_wrap(false);
   label35->set_use_markup(false);
   label35->set_selectable(false);
   label35->set_ellipsize(Pango::ELLIPSIZE_NONE);
   label35->set_width_chars(-1);
   label35->set_angle(0);
   label35->set_single_line_mode(false);
   hbox29->pack_start(*image235, Gtk::PACK_SHRINK, 0);
   hbox29->pack_start(*label35, Gtk::PACK_SHRINK, 0);
   alignment17->add(*hbox29);
   addLabelButton->set_flags(Gtk::CAN_FOCUS);
   addLabelButton->set_flags(Gtk::CAN_DEFAULT);
   addLabelButton->set_border_width(4);
   addLabelButton->set_relief(Gtk::RELIEF_NORMAL);
   addLabelButton->add(*alignment17);
   image236->set_alignment(0.5,0.5);
   image236->set_padding(0,0);
   label36->set_alignment(0.5,0.5);
   label36->set_padding(0,0);
   label36->set_justify(Gtk::JUSTIFY_LEFT);
   label36->set_line_wrap(false);
   label36->set_use_markup(false);
   label36->set_selectable(false);
   label36->set_ellipsize(Pango::ELLIPSIZE_NONE);
   label36->set_width_chars(-1);
   label36->set_angle(0);
   label36->set_single_line_mode(false);
   hbox30->pack_start(*image236, Gtk::PACK_SHRINK, 0);
   hbox30->pack_start(*label36, Gtk::PACK_SHRINK, 0);
   alignment18->add(*hbox30);
   removeLabelButton->set_flags(Gtk::CAN_FOCUS);
   removeLabelButton->set_flags(Gtk::CAN_DEFAULT);
   removeLabelButton->set_border_width(4);
   removeLabelButton->set_relief(Gtk::RELIEF_NORMAL);
   removeLabelButton->add(*alignment18);
   labelsVbuttonbox->pack_start(*addLabelButton);
   labelsVbuttonbox->pack_start(*removeLabelButton);
   labelsHbox->pack_start(*labelsScrolledwindow);
   labelsHbox->pack_start(*labelsVbuttonbox, Gtk::PACK_SHRINK, 0);
   labelsVbox->pack_start(*indexLabelsLabel, Gtk::PACK_SHRINK, 4);
   labelsVbox->pack_start(*labelsHbox, Gtk::PACK_EXPAND_WIDGET, 4);
   labelsLabel->set_alignment(0.5,0.5);
   labelsLabel->set_padding(0,0);
   labelsLabel->set_justify(Gtk::JUSTIFY_LEFT);
   labelsLabel->set_line_wrap(false);
   labelsLabel->set_use_markup(false);
   labelsLabel->set_selectable(false);
   labelsLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   labelsLabel->set_width_chars(-1);
   labelsLabel->set_angle(0);
   labelsLabel->set_single_line_mode(false);
   directoriesLabel->set_alignment(0,0.5);
   directoriesLabel->set_padding(4,4);
   directoriesLabel->set_justify(Gtk::JUSTIFY_LEFT);
   directoriesLabel->set_line_wrap(true);
   directoriesLabel->set_use_markup(false);
   directoriesLabel->set_selectable(false);
   directoriesLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   directoriesLabel->set_width_chars(-1);
   directoriesLabel->set_angle(0);
   directoriesLabel->set_single_line_mode(false);
   directoriesTreeview->set_flags(Gtk::CAN_FOCUS);
   directoriesTreeview->set_headers_visible(true);
   directoriesTreeview->set_rules_hint(false);
   directoriesTreeview->set_reorderable(false);
   directoriesTreeview->set_enable_search(true);
   directoriesScrolledwindow->set_flags(Gtk::CAN_FOCUS);
   directoriesScrolledwindow->set_border_width(4);
   directoriesScrolledwindow->set_shadow_type(Gtk::SHADOW_NONE);
   directoriesScrolledwindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   directoriesScrolledwindow->property_window_placement().set_value(Gtk::CORNER_TOP_LEFT);
   directoriesScrolledwindow->add(*directoriesTreeview);
   image609->set_alignment(0.5,0.5);
   image609->set_padding(0,0);
   label58->set_alignment(0.5,0.5);
   label58->set_padding(0,0);
   label58->set_justify(Gtk::JUSTIFY_LEFT);
   label58->set_line_wrap(false);
   label58->set_use_markup(false);
   label58->set_selectable(false);
   label58->set_ellipsize(Pango::ELLIPSIZE_NONE);
   label58->set_width_chars(-1);
   label58->set_angle(0);
   label58->set_single_line_mode(false);
   hbox54->pack_start(*image609, Gtk::PACK_SHRINK, 0);
   hbox54->pack_start(*label58, Gtk::PACK_SHRINK, 0);
   alignment35->add(*hbox54);
   addDirectoryButton->set_flags(Gtk::CAN_FOCUS);
   addDirectoryButton->set_flags(Gtk::CAN_DEFAULT);
   addDirectoryButton->set_border_width(4);
   addDirectoryButton->set_relief(Gtk::RELIEF_NORMAL);
   addDirectoryButton->add(*alignment35);
   removeDirectoryButton->set_flags(Gtk::CAN_FOCUS);
   removeDirectoryButton->set_flags(Gtk::CAN_DEFAULT);
   removeDirectoryButton->set_border_width(4);
   removeDirectoryButton->set_relief(Gtk::RELIEF_NORMAL);
   directoriesVbuttonbox->pack_start(*addDirectoryButton);
   directoriesVbuttonbox->pack_start(*removeDirectoryButton);
   directoriesHbox->pack_start(*directoriesScrolledwindow);
   directoriesHbox->pack_start(*directoriesVbuttonbox, Gtk::PACK_SHRINK, 0);
   patternsLabel->set_alignment(0,0.5);
   patternsLabel->set_padding(4,4);
   patternsLabel->set_justify(Gtk::JUSTIFY_LEFT);
   patternsLabel->set_line_wrap(true);
   patternsLabel->set_use_markup(false);
   patternsLabel->set_selectable(false);
   patternsLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   patternsLabel->set_width_chars(-1);
   patternsLabel->set_angle(0);
   patternsLabel->set_single_line_mode(false);
   patternsTreeview->set_flags(Gtk::CAN_FOCUS);
   patternsTreeview->set_headers_visible(true);
   patternsTreeview->set_rules_hint(false);
   patternsTreeview->set_reorderable(false);
   patternsTreeview->set_enable_search(true);
   patternsScrolledwindow->set_flags(Gtk::CAN_FOCUS);
   patternsScrolledwindow->set_border_width(4);
   patternsScrolledwindow->set_shadow_type(Gtk::SHADOW_NONE);
   patternsScrolledwindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   patternsScrolledwindow->property_window_placement().set_value(Gtk::CORNER_TOP_LEFT);
   patternsScrolledwindow->add(*patternsTreeview);
   patternsVbox->pack_start(*patternsScrolledwindow);
   patternsVbox->pack_start(*patternsCombobox, Gtk::PACK_SHRINK, 4);
   image624->set_alignment(0.5,0.5);
   image624->set_padding(0,0);
   label60->set_alignment(0.5,0.5);
   label60->set_padding(0,0);
   label60->set_justify(Gtk::JUSTIFY_LEFT);
   label60->set_line_wrap(false);
   label60->set_use_markup(false);
   label60->set_selectable(false);
   label60->set_ellipsize(Pango::ELLIPSIZE_NONE);
   label60->set_width_chars(-1);
   label60->set_angle(0);
   label60->set_single_line_mode(false);
   hbox56->pack_start(*image624, Gtk::PACK_SHRINK, 0);
   hbox56->pack_start(*label60, Gtk::PACK_SHRINK, 0);
   alignment36->add(*hbox56);
   addPatternButton->set_flags(Gtk::CAN_FOCUS);
   addPatternButton->set_flags(Gtk::CAN_DEFAULT);
   addPatternButton->set_border_width(4);
   addPatternButton->set_relief(Gtk::RELIEF_NORMAL);
   addPatternButton->add(*alignment36);
   removePatternButton->set_flags(Gtk::CAN_FOCUS);
   removePatternButton->set_flags(Gtk::CAN_DEFAULT);
   removePatternButton->set_border_width(4);
   removePatternButton->set_relief(Gtk::RELIEF_NORMAL);
   resetPatternsButton->set_flags(Gtk::CAN_FOCUS);
   resetPatternsButton->set_flags(Gtk::CAN_DEFAULT);
   resetPatternsButton->set_border_width(4);
   resetPatternsButton->set_relief(Gtk::RELIEF_NORMAL);
   patternsVbuttonbox->pack_start(*addPatternButton);
   patternsVbuttonbox->pack_start(*removePatternButton);
   patternsVbuttonbox->pack_start(*resetPatternsButton);
   patternsHbox->pack_start(*patternsVbox);
   patternsHbox->pack_start(*patternsVbuttonbox, Gtk::PACK_SHRINK, 0);
   indexingVbox->pack_start(*directoriesLabel, Gtk::PACK_SHRINK, 4);
   indexingVbox->pack_start(*directoriesHbox, Gtk::PACK_EXPAND_WIDGET, 4);
   indexingVbox->pack_start(*patternsLabel, Gtk::PACK_SHRINK, 4);
   indexingVbox->pack_start(*patternsHbox, Gtk::PACK_EXPAND_WIDGET, 4);
   indexingLabel->set_alignment(0.5,0.5);
   indexingLabel->set_padding(0,0);
   indexingLabel->set_justify(Gtk::JUSTIFY_LEFT);
   indexingLabel->set_line_wrap(false);
   indexingLabel->set_use_markup(false);
   indexingLabel->set_selectable(false);
   indexingLabel->set_ellipsize(Pango::ELLIPSIZE_NONE);
   indexingLabel->set_width_chars(-1);
   indexingLabel->set_angle(0);
   indexingLabel->set_single_line_mode(false);
   prefsNotebook->set_flags(Gtk::CAN_FOCUS);
   prefsNotebook->set_show_tabs(true);
   prefsNotebook->set_show_border(true);
   prefsNotebook->set_tab_pos(Gtk::POS_TOP);
   prefsNotebook->set_scrollable(false);
   prefsNotebook->append_page(*generalTable, *generalLabel);
   prefsNotebook->pages().back().set_tab_label_packing(false, true, Gtk::PACK_START);
   prefsNotebook->append_page(*netTable, *netLabel);
   prefsNotebook->pages().back().set_tab_label_packing(false, true, Gtk::PACK_START);
   prefsNotebook->append_page(*labelsVbox, *labelsLabel);
   prefsNotebook->pages().back().set_tab_label_packing(false, true, Gtk::PACK_START);
   prefsNotebook->append_page(*indexingVbox, *indexingLabel);
   prefsNotebook->pages().back().set_tab_label_packing(false, true, Gtk::PACK_START);
   prefsDialog->get_vbox()->set_homogeneous(false);
   prefsDialog->get_vbox()->set_spacing(0);
   prefsDialog->get_vbox()->pack_start(*prefsNotebook);
   prefsDialog->set_title(_("Preferences"));
   prefsDialog->set_modal(false);
   prefsDialog->property_window_position().set_value(Gtk::WIN_POS_NONE);
   prefsDialog->set_resizable(true);
   prefsDialog->property_destroy_with_parent().set_value(false);
   prefsDialog->set_has_separator(true);
   prefsDialog->add_action_widget(*prefsCancelbutton, -6);
   prefsDialog->add_action_widget(*prefsOkbutton, -5);
   prefsCancelbutton->show();
   prefsOkbutton->show();
   apiKeyLabel->show();
   apiKeyEntry->show();
   newResultsLabel->show();
   queriesLabel->show();
   enableCompletionCheckbutton->show();
   newResultsColorbutton->show();
   ignoreRobotsCheckbutton->show();
   robotsLabel->show();
   generalTable->show();
   generalLabel->show();
   directConnectionRadiobutton->show();
   proxyRadiobutton->show();
   proxyAddressLabel->show();
   proxyAddressEntry->show();
   proxyTypeLabel->show();
   proxyPortLabel->show();
   proxyPortSpinbutton->show();
   proxyTypeCombobox->show();
   socksTable->show();
   netTable->show();
   netLabel->show();
   indexLabelsLabel->show();
   labelsTreeview->show();
   labelsScrolledwindow->show();
   image235->show();
   label35->show();
   hbox29->show();
   alignment17->show();
   addLabelButton->show();
   image236->show();
   label36->show();
   hbox30->show();
   alignment18->show();
   removeLabelButton->show();
   labelsVbuttonbox->show();
   labelsHbox->show();
   labelsVbox->show();
   labelsLabel->show();
   directoriesLabel->show();
   directoriesTreeview->show();
   directoriesScrolledwindow->show();
   image609->show();
   label58->show();
   hbox54->show();
   alignment35->show();
   addDirectoryButton->show();
   removeDirectoryButton->show();
   directoriesVbuttonbox->show();
   directoriesHbox->show();
   patternsLabel->show();
   patternsTreeview->show();
   patternsScrolledwindow->show();
   patternsCombobox->show();
   patternsVbox->show();
   image624->show();
   label60->show();
   hbox56->show();
   alignment36->show();
   addPatternButton->show();
   removePatternButton->show();
   resetPatternsButton->show();
   patternsVbuttonbox->show();
   patternsHbox->show();
   indexingVbox->show();
   indexingLabel->show();
   prefsNotebook->show();
   prefsDialog->show();
   prefsOkbutton->signal_clicked().connect(sigc::mem_fun(*this, &prefsDialog_glade::on_prefsOkbutton_clicked), false);
   directConnectionRadiobutton->signal_toggled().connect(sigc::mem_fun(*this, &prefsDialog_glade::on_directConnectionRadiobutton_toggled), false);
   addLabelButton->signal_clicked().connect(sigc::mem_fun(*this, &prefsDialog_glade::on_addLabelButton_clicked), false);
   removeLabelButton->signal_clicked().connect(sigc::mem_fun(*this, &prefsDialog_glade::on_removeLabelButton_clicked), false);
   addDirectoryButton->signal_clicked().connect(sigc::mem_fun(*this, &prefsDialog_glade::on_addDirectoryButton_clicked), false);
   removeDirectoryButton->signal_clicked().connect(sigc::mem_fun(*this, &prefsDialog_glade::on_removeDirectoryButton_clicked), false);
   patternsCombobox->signal_changed().connect(sigc::mem_fun(*this, &prefsDialog_glade::on_patternsCombobox_changed), false);
   addPatternButton->signal_clicked().connect(sigc::mem_fun(*this, &prefsDialog_glade::on_addPatternButton_clicked), false);
   removePatternButton->signal_clicked().connect(sigc::mem_fun(*this, &prefsDialog_glade::on_removePatternButton_clicked), false);
   resetPatternsButton->signal_clicked().connect(sigc::mem_fun(*this, &prefsDialog_glade::on_resetPatternsButton_clicked), false);
}

prefsDialog_glade::~prefsDialog_glade()
{  delete gmm_data;
}

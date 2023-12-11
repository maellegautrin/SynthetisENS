#include "menubar.h"
#include "wav.h"
#include "signal_utils.h"

using namespace synthetisens;

MenuBar::MenuBar() {
  //File menu
  file = Gtk::manage(new Gtk::MenuItem("_File", true));
  file_menu = Gtk::manage(new Gtk::Menu());
  play = Gtk::manage(new Gtk::MenuItem("_Play", true));
  save = Gtk::manage(new Gtk::MenuItem("_Save", true));
  duration = Gtk::manage(new Gtk::MenuItem("_Duration", true));

  play->signal_activate().connect(sigc::ptr_fun(play_speaker));
  save->signal_activate().connect(sigc::ptr_fun(save_speaker_wav));
  duration->signal_activate().connect(sigc::ptr_fun(change_duration));
  
  //Edit menu
  edit = Gtk::manage(new Gtk::MenuItem("_Edit", true));


  //View menu
  view = Gtk::manage(new Gtk::MenuItem("_View", true));
  view_menu = Gtk::manage(new Gtk::Menu());
  preview = Gtk::manage(new Gtk::MenuItem("_Preview", true));

  preview->signal_activate().connect(sigc::ptr_fun(preview_speaker));


  //Append
  file->set_submenu(*file_menu);
  file_menu->append(*play);
  file_menu->append(*save);
  file_menu->append(*duration);
  
  view->set_submenu(*view_menu);
  view_menu->append(*preview);

  append(*file);
  append(*edit);
  append(*view);
}

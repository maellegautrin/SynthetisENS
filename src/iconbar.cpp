#include "iconbar.h"

#include "gtkmm/stock.h"
#include "signal_utils.h"
#include "wav.h"

using namespace synthetisens;

IconBar::IconBar() {
  this->set_icon_size(Gtk::ICON_SIZE_LARGE_TOOLBAR);

  play = Gtk::manage(new Gtk::ToolButton(Gtk::Stock::MEDIA_PLAY));
  play->signal_clicked().connect(sigc::ptr_fun(&play_speaker));

  stop = Gtk::manage(new Gtk::ToolButton(Gtk::Stock::MEDIA_STOP));
  stop->signal_clicked().connect(sigc::ptr_fun(&stop_signal));

  save = Gtk::manage(new Gtk::ToolButton(Gtk::Stock::SAVE));
  save->signal_clicked().connect(sigc::ptr_fun(&save_speaker_wav));


  append(*play);
  append(*stop);
  append(*save);
}

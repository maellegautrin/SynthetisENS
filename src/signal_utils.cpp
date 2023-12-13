#include "signal_utils.h"
#include "RtWvOut.h"
#include "component.h"
#include "gtkmm/dialog.h"
#include "gtkmm/image.h"
#include "gtkmm/frame.h"
#include "signal_viewer.h"
#include "window.h"
#include "gtkmm/entry.h"

#include <thread>

extern synthetisens::speaker_component* speaker;
extern synthetisens::Window* window;
extern int duration;
extern bool playing;

void play_signal(synthetisens::signal& sig) {
  stk::RtWvOut play_sample = stk::RtWvOut(1);

  sig.reset();
  for (int i = 0; i < sig.size && playing; i++) {
    play_sample.tick(sig.tick());
  }

  playing = false;
}

void play_speaker_thread() {
  synthetisens::signal* sig = speaker->generate_output(0, duration).value.signal;
  sig->reset();

  stk::RtWvOut play_sample = stk::RtWvOut(1);

  for (int i = 0; i < sig->size && playing; i++) {
    if (speaker->update_signal) {
      int pos = sig->get_position();
      sig = speaker->generate_output(0, duration).value.signal;
      sig->set_position(pos);
      speaker->update_signal = false;
    }

    play_sample.tick(sig->tick());
  }

  playing = false;


  // play_signal(*speaker->generate_output(0, duration).value.signal);
}

void play_speaker() {
  if (playing) return;
  playing = true;
  std::thread t(play_speaker_thread);
  t.detach();
}

void stop_signal() {
  playing = false;
}

void preview_speaker() {
  Dialog* pop = new Dialog();
  pop->set_title("Speaker preview");
  pop->set_default_size(1000, 500);
  pop->set_transient_for(*window);
  pop->set_modal(true);
  pop->set_resizable(false);
  pop->set_position(Gtk::WIN_POS_CENTER);

  Image* img = new Image("img/speaker.png");
  img->set_valign(Gtk::ALIGN_CENTER);
  img->set_halign(Gtk::ALIGN_CENTER);
  pop->get_content_area()->add(*img);

  Frame* frame = new Frame();

  synthetisens::SignalViewer* sigview = new synthetisens::SignalViewer(1000, 500, speaker->generate_output(0, duration).value.signal);
  frame->add(*sigview);

  pop->get_content_area()->add(*frame);

  pop->show_all_children();
  pop->show();
}

void change_duration() {
   Gtk::Dialog* dialog = new Gtk::Dialog();
    dialog->set_title("Change Duration");
    dialog->add_button("OK", Gtk::RESPONSE_OK);
    dialog->add_button("Cancel", Gtk::RESPONSE_CANCEL);
    Gtk::Entry* entry = new Gtk::Entry();
    entry->set_text(std::to_string(duration/SAMPLE_FREQ));
    dialog->get_content_area()->add(*entry);
    dialog->show_all();
    int result = dialog->run();
    dialog->close();
    if (result == Gtk::RESPONSE_OK) {
      duration = SAMPLE_FREQ*std::atoi(entry->get_text().data());
      
    }
}

// Forcing OS to linux (useful for Stk)
#define __OS_LINUX__

#include <gtkmm.h>

#include <iostream>

#include "SineWave.h"
#include "RtWvOut.h"

using namespace stk;
using namespace Gtk;

// Gtk components
Window* window;
Box* box;
Button* button;
Scale* scale;

RtWvOut* dac;


// Function prototypes
void play_sound();



// Main function
int main(int argc, char *argv[])
{
  auto app = Application::create(argc, argv, "synthetisens.app");

  window = new Window();
  window->set_default_size(200, 200);
  window->set_title("synthetisENS");

  box = new Box(ORIENTATION_VERTICAL);

  button = new Button("Play sound");
  button->signal_clicked().connect(sigc::ptr_fun(&play_sound));
  box->add(*button);

  scale = new Scale(ORIENTATION_HORIZONTAL);
  scale->set_range(0.0, 1000.0);
  box->add(*scale);

  window->add(*box);
  window->show_all_children();
  

  Stk::setSampleRate(44100.0);
  Stk::showWarnings(true);

  return app->run(*window);
}


// Function definitions
void play_sound()
{
  dac = new RtWvOut( 1 );
  SineWave sine;
  sine.setFrequency(scale->get_value());
  for (int i = 0; i < 70000; i++) dac->tick(sine.tick());
  delete dac;
}

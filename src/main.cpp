// Forcing OS to linux (useful for Stk)
#define __OS_LINUX__

#define sine_id 1
#define square_id 2
#define sawtooth_id 3
#define blit_id 4



#include <gtkmm.h>

#include <iostream>


#include <Blit.h>
#include <BlitSaw.h>
#include <BlitSquare.h>
#include "SineWave.h"
#include "RtWvOut.h"

using namespace stk;
using namespace Gtk;

// Gtk components
Window* window;
Box* box;
Button* button;
Button* sinebutton;
Button* squarebutton;
Button* sawbutton;
Button* blitbutton;
Scale* freq;
Scale* duration;
Scale* harmo;

int wave = sine_id;

RtWvOut* dac;


// Function prototypes
void play_sound();
void set_sine();
void set_square();
void set_saw();
void set_blit();



// Main function
int main(int argc, char *argv[])
{
  auto app = Application::create(argc, argv, "synthetisens.app");
  wave = sine_id;

  window = new Window();
  window->set_default_size(200, 200);
  window->set_title("synthetisENS");

  box = new Box(ORIENTATION_VERTICAL);

  button = new Button("Play sound");
  button->signal_clicked().connect(sigc::ptr_fun(&play_sound));
  box->add(*button);

  sinebutton = new Button("Sine wave");
  sinebutton->signal_clicked().connect(sigc::ptr_fun(&set_sine));
  box->add(*sinebutton);

  squarebutton = new Button("Square wave");
  squarebutton->signal_clicked().connect(sigc::ptr_fun(&set_square));
  box->add(*squarebutton);

  sawbutton = new Button("Sawtooth wave");
  sawbutton->signal_clicked().connect(sigc::ptr_fun(&set_saw));
  box->add(*sawbutton);

  blitbutton = new Button("BLIT");
  blitbutton->signal_clicked().connect(sigc::ptr_fun(&set_blit));
  box->add(*blitbutton);

  freq = new Scale(ORIENTATION_HORIZONTAL);
  freq->set_range(0.0, 1000.0);
  box->add(*freq);

  duration = new Scale(ORIENTATION_HORIZONTAL);
  duration->set_range(0.0, 100000.0);
  box->add(*duration);

  harmo = new Scale(ORIENTATION_HORIZONTAL);
  harmo->set_range(0,10);
  harmo->set_increments(1,1);
  box->add(*harmo);

  window->add(*box);
  window->show_all_children();
  

  Stk::setSampleRate(44100.0);
  Stk::showWarnings(true);

  return app->run(*window);
}


void set_sine(){
  wave = sine_id;
}


void set_square(){
  wave = square_id;
}


void set_saw(){
  wave = sawtooth_id;
}

void set_blit(){
  wave = blit_id;
}

// Function definitions
void play_sound()
{
  dac = new RtWvOut( 1 );
  BlitSquare square;
  BlitSaw sawtooth;
  Blit blit;
  SineWave sine;
  if(wave == sine_id){
    sine.setFrequency(freq->get_value());
    for (int i = 0; i < duration->get_value(); i++) dac->tick(sine.tick());
  }
  if(wave == square_id){
    square.setFrequency(freq->get_value());
    for (int i = 0; i < duration->get_value(); i++) dac->tick(square.tick());
  }
  if(wave == sawtooth_id){
    sawtooth.setFrequency(freq->get_value());
    for (int i = 0; i < duration->get_value(); i++) dac->tick(sawtooth.tick());
  }
  if(wave == blit_id){
    blit.setFrequency(freq->get_value());
    blit.setHarmonics(harmo->get_value());
    for (int i = 0; i < duration->get_value(); i++) dac->tick(blit.tick());
  }
  delete dac;
}

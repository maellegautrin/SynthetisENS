// Forcing OS to linux (useful for Stk)
#include "gtkmm/enums.h"
#define __OS_LINUX__

#define sine_id 1
#define square_id 2
#define sawtooth_id 3
#define blit_id 4



#include <gtkmm.h>

#include <iostream>

#include "component.h"
#include "componentselector.h"
#include "componenteffective.h"

#include <Blit.h>
#include <BlitSaw.h>
#include <BlitSquare.h>
#include "SineWave.h"
#include "RtWvOut.h"

using namespace stk;
using namespace Gtk;
using namespace std;

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

/*-----------------INTERFACE------------------*/
// DELCARATIONS

  Grid* main_grid;
  ScrolledWindow* sigs;
  Grid* sig_grid;
  ScrolledWindow* filters;
  Grid* filter_grid;
  ScrolledWindow* operators;
  Grid* operator_grid;
  ScrolledWindow* other;
  Grid* other_grid;
  DrawingArea* keyboard;
  Frame* components_frame;
  Frame* workspace_frame;
  Grid* synth_grid;
  Notebook* components;
  Notebook* workspace;
  MenuBar* menubar;
  MenuItem* file;
  MenuItem* edit;
  MenuItem* view;

//SETUP
void layout_setup(){
  main_grid = new Grid();
  sigs = new ScrolledWindow();
  sig_grid = new Grid();
  filters = new ScrolledWindow();
  filter_grid = new Grid();
  operators = new ScrolledWindow();
  operator_grid = new Grid();
  other = new ScrolledWindow();
  other_grid = new Grid();
  keyboard = new DrawingArea();
  components_frame = new Frame("components");
  workspace_frame = new Frame("workspace");
  synth_grid = new Grid();
  components = new Notebook();
  workspace = new Notebook();
  menubar = new MenuBar();
  file = new MenuItem("file");
  edit = new MenuItem("edit");
  view = new MenuItem("view");

  components->append_page(*sigs, "signals");
  components->append_page(*filters, "filters");
  components->append_page(*operators, "operators");
  components->append_page(*other,"other");
  workspace->append_page(*synth_grid, "synth");
  workspace->append_page(*keyboard, "keyboard");

  sigs->add(*sig_grid);
  filters->add(*filter_grid);
  operators->add(*operator_grid);
  other->add(*other_grid);


  components_frame->add(*components);
  workspace_frame->add(*workspace);

  sigs->set_size_request(250,200);
  synth_grid->set_size_request(800,600);


  menubar->append(*file);
  menubar->append(*edit);
  menubar->append(*view);

  main_grid->attach(*menubar,0,0,3,1);
  main_grid->attach_next_to(*components_frame,*menubar,POS_BOTTOM,3,3);
  main_grid->attach_next_to(*workspace_frame,*components_frame,POS_RIGHT,3,3);
};



/*--------------------------------------------*/
/*------------------COMPONENTS----------------*/
synthetisens::ComponentSelector** signal_list;
synthetisens::ComponentSelector** filter_list;
synthetisens::ComponentSelector** operator_list;
synthetisens::ComponentSelector** other_list;
/*--------------------------------------------*/





// Function prototypes
void play_sound();
void set_sine();
void set_square();
void set_saw();
void set_blit();



// Main function
int main(int argc, char *argv[])
{
  std::cout << Stk::sampleRate() << std::endl;
  auto app = Application::create(argc, argv, "synthetisens.app");
  wave = sine_id;

  window = new Window();
  window->set_default_size(1500, 1000);
  window->set_title("synthetisENS");

  layout_setup();

  synthetisens::ComponentSelector* sine_selector = new synthetisens::ComponentSelector("sine.png",synthetisens::SIGNAL_COMPONENT,0);
  synthetisens::ComponentSelector* square_selector = new synthetisens::ComponentSelector("square.png",synthetisens::SIGNAL_COMPONENT,1);
  synthetisens::ComponentSelector* triangle_selector = new synthetisens::ComponentSelector("triangle.png",synthetisens::SIGNAL_COMPONENT,2);

  Gtk::Frame* sine_frame = new Frame();
  sine_frame->add(*sine_selector);
  Gtk::Frame* square_frame = new Frame();
  square_frame->add(*square_selector);
  Gtk::Frame* triangle_frame = new Frame();
  triangle_frame->add(*triangle_selector);

  sig_grid->attach(*sine_frame,0,0,1,1);
  sig_grid->attach_next_to(*square_frame,*sine_frame,POS_RIGHT,1,1);
  sig_grid->attach_next_to(*triangle_frame,*sine_frame,POS_BOTTOM,1,1);




  synthetisens::ComponentEffective* sine_eff = new synthetisens::ComponentEffective("sine.png",synthetisens::SIGNAL_COMPONENT,0);

  synth_grid->attach(*sine_eff,1,1,1,1);

  /*box = new Box(ORIENTATION_VERTICAL);

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
  
*/
  Stk::setSampleRate(44100.0);
  Stk::showWarnings(true);

  window->add(*main_grid);
  window->show_all_children();

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
// void play_sound()
// {
//   dac = new RtWvOut( 1 );
//   BlitSquare square;
//   BlitSaw sawtooth;
//   Blit blit;
//   SineWave sine;
//   if(wave == sine_id){
//     sine.setFrequency(freq->get_value());
//     for (int i = 0; i < duration->get_value(); i++) dac->tick(sine.tick());
//   }
//   if(wave == square_id){
//     square.setFrequency(freq->get_value());
//     for (int i = 0; i < duration->get_value(); i++) dac->tick(square.tick());
//   }
//   if(wave == sawtooth_id){
//     sawtooth.setFrequency(freq->get_value());
//     for (int i = 0; i < duration->get_value(); i++) dac->tick(sawtooth.tick());
//   }
//   if(wave == blit_id){
//     blit.setFrequency(freq->get_value());
//     blit.setHarmonics(harmo->get_value());
//     for (int i = 0; i < duration->get_value(); i++) dac->tick(blit.tick());
//   }
//   delete dac;
// }


void play_sound()
{
  dac = new RtWvOut( 1 );
  synthetisens::component* freq = new synthetisens::constant_component(440);
  synthetisens::component* ampl = new synthetisens::constant_component(1);
  synthetisens::component* phase = new synthetisens::constant_component(0);
  
  synthetisens::component* c1 = new synthetisens::sinusoidal_component();
  c1->connect_input(0, *freq, 0);
  c1->connect_input(1, *ampl, 0);
  c1->connect_input(2, *phase, 0);

  synthetisens::signal* sine = c1->generate_output(0).value.signal;

  for (int i = 0; i < duration->get_value(); i++) dac->tick(sine->tick());

  delete dac;
}

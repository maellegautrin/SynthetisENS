// Forcing OS to linux (useful for Stk)
#include "circuit_area.h"
#include "gtkmm/enums.h"
#include "gtkmm/widget.h"
#define __OS_LINUX__

#define sine_id 1
#define square_id 2
#define sawtooth_id 3
#define blit_id 4

#define LINES 6
#define COLUMNS 6



#include <gtkmm.h>

#include <iostream>

#include "component.h"
#include "componentselector.h"
#include "componenteffective.h"
#include "signal_viewer.h"
#include "component_definition.h"

#include "RtWvOut.h"

using namespace stk;
using namespace Gtk;
using namespace std;
using namespace synthetisens;

//Port label variables
synthetisens::Port* last_clicked;
int port_label;
const char* label_space = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

//Selector variables
synthetisens::ComponentSelector* current_selector;

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

RtWvOut* dac;

void play_sound();
void preview_speaker();

synthetisens::component* speaker;

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
  Box* keyboard;
  Frame* components_frame;
  Frame* workspace_frame;
  synthetisens::CircuitArea* circuit_area;
  Notebook* components;
  Notebook* workspace;
  MenuBar* menubar;

  MenuItem* file;
  Menu* file_menu;
  MenuItem* play;

  MenuItem* edit;

  MenuItem* view;
  Menu* view_menu;
  MenuItem* preview;

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
  keyboard = new Box();
  components_frame = new Frame("components");
  workspace_frame = new Frame("workspace");
  circuit_area = new synthetisens::CircuitArea();
  components = new Notebook();
  workspace = new Notebook();
  menubar = new MenuBar();
  file = new MenuItem("file");
  file_menu = new Menu();
  play = new MenuItem("play");
  edit = new MenuItem("edit");
  view = new MenuItem("view");
  view_menu = new Menu();
  preview = new MenuItem("preview speaker");


  Image* keyboardimg = new Image("WIP.png");
  keyboard->add(*keyboardimg);

  components->append_page(*sigs, "signals");
  components->append_page(*filters, "filters");
  components->append_page(*operators, "operators");
  components->append_page(*other,"other");
  workspace->append_page(*circuit_area, "circuit");
  workspace->append_page(*keyboard, "keyboard");

  sigs->add(*sig_grid);
  filters->add(*filter_grid);
  operators->add(*operator_grid);
  other->add(*other_grid);


  components_frame->add(*components);
  workspace_frame->add(*workspace);

  sigs->set_size_request(375,400);
  circuit_area->set_size_request(900,600);

  play->signal_activate().connect(sigc::ptr_fun(&play_sound));
  file_menu->append(*play);
  file->set_submenu(*file_menu);
  
  preview->signal_activate().connect(sigc::ptr_fun(&preview_speaker));
  view_menu->append(*preview);
  view->set_submenu(*view_menu);

  menubar->append(*file);
  menubar->append(*edit);
  menubar->append(*view);

  main_grid->attach(*menubar,0,0,3,1);
  main_grid->attach_next_to(*components_frame,*menubar,POS_BOTTOM,3,3);
  main_grid->attach_next_to(*workspace_frame,*components_frame,POS_RIGHT,3,3);

}



/*--------------------------------------------*/
/*------------------COMPONENTS----------------*/
synthetisens::ComponentSelector** signal_list;
synthetisens::ComponentSelector** filter_list;
synthetisens::ComponentSelector** operator_list;
synthetisens::ComponentSelector** other_list;

/*--------------------------------------------*/
/*-----------------SELECTORS------------------*/

//SIGNALS
ComponentSelector* sine_selector;
synthetisens::ComponentSelector* square_selector;
synthetisens::ComponentSelector* triangle_selector;
synthetisens::ComponentSelector* sawtooth_selector;
//FILTERS

//OPERATORS
synthetisens::ComponentSelector* sum_selector;
synthetisens::ComponentSelector* sub_selector;
synthetisens::ComponentSelector* prod_selector;
synthetisens::ComponentSelector* div_selector;
synthetisens::ComponentSelector* derivative_selector;
synthetisens::ComponentSelector* primitive_selector;
synthetisens::ComponentSelector* normalize_selector;
//OTHER
synthetisens::ComponentSelector* speaker_selector;
synthetisens::ComponentSelector* keyboard_selector;
synthetisens::ComponentSelector* knob_selector;
synthetisens::ComponentSelector* slider_selector;
synthetisens::ComponentSelector* const0_selector;
synthetisens::ComponentSelector* const1_selector;
synthetisens::ComponentSelector* const440_selector;
synthetisens::ComponentSelector* const880_selector;


void selector_setup() {
  //SIGNALS
  sine_selector = new ComponentSelector(SINUSOIDAL_GENERATOR);
  square_selector = new ComponentSelector(SQUARE_GENERATOR);
  triangle_selector = new ComponentSelector(TRIANGLE_GENERATOR);
  sawtooth_selector = new ComponentSelector(SAWTOOTH_GENERATOR);

  //FILTERS

  // //OPERATORS
  sum_selector = new ComponentSelector(SUM);
  sub_selector = new ComponentSelector(SUB);
  prod_selector = new ComponentSelector(MUL);
  div_selector = new ComponentSelector(DIV);
  derivative_selector = new ComponentSelector(DERIVATIVE);
  primitive_selector = new ComponentSelector(PRIMITIVE);
  // normalize_selector = new ComponentSelector(NORMALIZE);
  
  //OTHER
  speaker_selector = new ComponentSelector(SPEAKER);
  // keyboard_selector = new ComponentSelector(KEYBOARD);
  // knob_selector = new ComponentSelector(KNOB);
  // slider_selector = new ComponentSelector(SLIDER);
  // const0_selector = new ComponentSelector(CONSTANT);
  const1_selector = new ComponentSelector(CONSTANT1);
  const440_selector = new ComponentSelector(CONSTANT440);
  // const880_selector = new ComponentSelector(CONSTANT);

  // cout << sine_selector << endl;
  sig_grid->attach(*sine_selector,1,1,1,1);
  sig_grid->attach(*square_selector,2,1,1,1);
  sig_grid->attach(*triangle_selector,3,1,1,1);
  sig_grid->attach(*sawtooth_selector,1,2,1,1);

  // //filter_grid->attach

  operator_grid->attach(*sum_selector,1,1,1,1);
  operator_grid->attach(*sub_selector,2,1,1,1);
  operator_grid->attach(*prod_selector,3,1,1,1);
  operator_grid->attach(*div_selector,1,2,1,1);
  operator_grid->attach(*derivative_selector,2,2,1,1);
  operator_grid->attach(*primitive_selector,3,2,1,1);
  // operator_grid->attach(*normalize_selector,1,3,1,1);

  other_grid->attach(*speaker_selector,1,1,1,1);
  // other_grid->attach(*keyboard_selector,2,1,1,1);
  // other_grid->attach(*knob_selector,3,1,1,1);
  // other_grid->attach(*slider_selector,1,2,1,1);
  // other_grid->attach(*const0_selector,2,2,1,1);
  other_grid->attach(*const1_selector,3,2,1,1);
  other_grid->attach(*const440_selector,1,3,1,1);
  // other_grid->attach(*const880_selector,2,3,1,1);

}

/*--------------------------------------------*/

// Main function
int main(int argc, char *argv[])
{
  std::cout << Stk::sampleRate() << std::endl;
  auto app = Application::create(argc, argv, "synthetisens.app");

  window = new Window();
  window->set_default_size(1500, 1000);
  window->set_title("synthetisENS");

  speaker = new synthetisens::speaker_component();

  layout_setup();
  //synth_grid_setup();
  selector_setup();

  last_clicked = NULL;
  port_label = 0;
  current_selector = NULL;



  Stk::setSampleRate(44100.0);
  Stk::showWarnings(true);

  window->add(*main_grid);
  window->show_all_children();

  return app->run(*window);
}

// Function definitions
void play_sound()
{
  dac = new RtWvOut( 1 );
  synthetisens::signal* sig = speaker->generate_output(0).value.signal;

  for (int i = 0; i < 50000; i++) dac->tick(sig->tick());

  delete dac;
}

void preview_speaker()
{
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

  synthetisens::SignalViewer* sigview = new synthetisens::SignalViewer(1000, 500, speaker->generate_output(0).value.signal);
  frame->add(*sigview);

  pop->get_content_area()->add(*frame);

  pop->show_all_children();
  pop->show();
}

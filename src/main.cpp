// Forcing OS to linux (useful for Stk)
#include "circuit_area.h"
#include "gdkmm/dragcontext.h"
#include "gdkmm/types.h"
#include "gtkmm/drawingarea.h"
#include "gtkmm/enums.h"
#include "gtkmm/iconset.h"
#include "gtkmm/popover.h"
#include "gtkmm/selectiondata.h"
#include "gtkmm/targetentry.h"
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
#include "gridslot.h"
#include "signal_viewer.h"
#include "component_definition.h"

#include <Blit.h>
#include <BlitSaw.h>
#include <BlitSquare.h>
#include "SineWave.h"
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

int wave = sine_id;

RtWvOut* dac;

void play_sound();
void preview_speaker();

synthetisens::component* speaker;

bool on_drag_motion(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, guint time){
  std::cout << "Drag motion" << std::endl;
  return true;
}

void on_drag_leave(const Glib::RefPtr<Gdk::DragContext>& context, guint time){
  std::cout << "Drag leave" << std::endl;
}

bool on_drag_drop(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, guint time){
  std::cout << "Drag drop dest" << std::endl;
  return true;
}

void on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time){
  std::cout << "Drag data received " << x << " - " << y << std::endl;
  std::string data = selection_data.get_data_as_string();
  std::cout << data << std::endl;
}

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
  Grid* synth_grid;
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
  synth_grid = new Grid();
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

  synthetisens::CircuitArea* circuit_area = new synthetisens::CircuitArea();

  components->append_page(*sigs, "signals");
  components->append_page(*filters, "filters");
  components->append_page(*operators, "operators");
  components->append_page(*other,"other");
  workspace->append_page(*synth_grid, "synth");
  workspace->append_page(*keyboard, "keyboard");
  workspace->append_page(*circuit_area, "circuit");

  sigs->add(*sig_grid);
  filters->add(*filter_grid);
  operators->add(*operator_grid);
  other->add(*other_grid);


  components_frame->add(*components);
  workspace_frame->add(*workspace);

  sigs->set_size_request(375,400);
  synth_grid->set_size_request(900,600);
  synth_grid->drag_dest_set(std::vector<Gtk::TargetEntry>({Gtk::TargetEntry("INTEGER")}), DEST_DEFAULT_ALL, Gdk::ACTION_COPY);
  synth_grid->signal_drag_motion().connect(sigc::ptr_fun(&on_drag_motion));
  synth_grid->signal_drag_leave().connect(sigc::ptr_fun(&on_drag_leave));
  synth_grid->signal_drag_drop().connect(sigc::ptr_fun(&on_drag_drop));
  synth_grid->signal_drag_data_received().connect(sigc::ptr_fun(&on_drag_data_received));

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
/*--------------------GRID--------------------*/
Frame* synth_grid_quartering[COLUMNS][LINES];

/*--------------------------------------------*/
/*-----------------DRAG&DROP------------------
enum {
    TARGET_COMPONENT_TYPE,
    TARGET_COMPONENT_ID,
    TARGET_IMG_LINK
};

std::vector<TargetEntry> target_list = {
  TargetEntry("TYPE", TARGET_SAME_APP,TARGET_COMPONENT_TYPE),
  TargetEntry("ID",TARGET_SAME_APP,TARGET_COMPONENT_ID),
  TargetEntry("IMG",TARGET_SAME_APP,TARGET_IMG_LINK)
};

void drag_data_received_handl(Widget* widget, GdkDragContext *context, gint x, gint y, SelectionData *selection_data, guint target_type, guint time, gpointer data){


    std::cout << "Destination " << 1 << ": Received data for the '" << selection_data->get_target() << "' target." << endl;

}




void drag_and_drop_destination_setup(Widget* destination){
  destination->drag_dest_set(target_list, DEST_DEFAULT_ALL, Gdk::ACTION_COPY);


  destination->signal_connect("drag-data-received", G_CALLBACK(drag_data_received_handl), NULL);
  g_signal_connect(*destination, "drag-leave", G_CALLBACK (drag_leave_handl), NULL);
  g_signal_connect(*destination, "drag-motion", G_CALLBACK (drag_motion_handl), NULL);
  g_signal_connect(*destination, "drag-drop", G_CALLBACK (drag_drop_handl), NULL);*//*

}

void drag_and_drop_source_setup(Widget* source){
  source->drag_source_set(target_list,Gdk::BUTTON1_MASK,Gdk::ACTION_COPY);


  g_signal_connect(source, "drag-data-get", G_CALLBACK (drag_data_get_handl), NULL);
  g_signal_connect(source, "drag-data-delete", G_CALLBACK (drag_data_delete_handl), NULL);
  g_signal_connect(source, "drag-begin", G_CALLBACK (drag_begin_handl), NULL);
  g_signal_connect(source, "drag-end", G_CALLBACK (drag_end_handl), NULL);*/

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






// Function prototypes
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

  speaker = new synthetisens::speaker_component();

  layout_setup();
  //synth_grid_setup();
  selector_setup();

  auto synth_grid_quartering = new Frame* [COLUMNS][LINES] ;
  for(int i = 0; i < COLUMNS; i++){
    for(int j = 0; j < LINES; j++){
      synth_grid_quartering[i][j] = new synthetisens::GridSlot(synth_grid, i, j);
      
    }
  }

  last_clicked = NULL;
  port_label = 0;
  current_selector = NULL;

  /*synthetisens::ComponentSelector* sine_selector = new synthetisens::ComponentSelector("sine.png",synthetisens::SIGNAL_COMPONENT,0);
  synthetisens::ComponentSelector* square_selector = new synthetisens::ComponentSelector("square.png",synthetisens::SIGNAL_COMPONENT,1);
  synthetisens::ComponentSelector* triangle_selector = new synthetisens::ComponentSelector("triangle.png",synthetisens::SIGNAL_COMPONENT,2);

  //drag_and_drop_destination_setup(synth_grid_quartering[0][0]);
  //drag_and_drop_source_setup(sine_selector);

  Gtk::Frame* sine_frame = new Frame();
  sine_frame->add(*sine_selector);
  Gtk::Frame* square_frame = new Frame();
  square_frame->add(*square_selector);
  Gtk::Frame* triangle_frame = new Frame();
  triangle_frame->add(*triangle_selector);

  sig_grid->attach(*sine_frame,0,0,1,1);
  sig_grid->attach_next_to(*square_frame,*sine_frame,POS_RIGHT,1,1);
  sig_grid->attach_next_to(*triangle_frame,*square_frame,POS_RIGHT,1,1);
  */




  //synthetisens::ComponentEffective* sine_eff = new synthetisens::ComponentEffective("sine.png",synthetisens::SIGNAL_COMPONENT,0);
  //synthetisens::ComponentEffective* sum_eff = new synthetisens::ComponentEffective("sum.png",synthetisens::OPERATOR_COMPONENT,0);


  //sine_eff->place(synth_grid_quartering[3][2]);
  //sum_eff->place(synth_grid_quartering[1][4]);

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
  // synthetisens::component* freq = new synthetisens::constant_component(440);
  // synthetisens::component* ampl = new synthetisens::constant_component(1);
  // synthetisens::component* phase = new synthetisens::constant_component(0);
  // 
  // synthetisens::component* c1 = new synthetisens::sinusoidal_component();
  // c1->connect_input(0, *freq, 0);
  // c1->connect_input(1, *ampl, 0);
  // c1->connect_input(2, *phase, 0);

  // synthetisens::signal* sine = c1->generate_output(0).value.signal;

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

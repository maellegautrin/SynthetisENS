#include "componenteffective.h"
#include "component.h"
#include "component_definition.h"
#include "gtkmm/enums.h"
#include "gtkmm/frame.h"
#include "gtkmm/grid.h"
#include "gtkmm/widget.h"
#include "gtkmm/image.h"
#include "gtkmm/window.h"
#include "gtkmm/box.h"
#include <cstring>
#include <vector>
#include "gtkmm/dialog.h"
#include "gtkmm/entry.h"
#include <sndfile.h>
#include "gtkmm/filechooserdialog.h"


extern Gtk::Window* window;

char* strremove(char* str, char letter){
    int removed = 0;
    const int n = strlen(str) - 1;
    char* newstr = new char[n];
    for(int i = 0; i < n; i++){
        if(str[i] == letter){
            removed = 1;
        }
        newstr[i] = str[i+removed];
    }
    return newstr;
}

char* strcopy(const char* str){
    char* copy = new char[strlen(str)];
    for(int i = 0; i < strlen(str); i++){
        copy[i] = str[i];
    }
    return copy;
}


using namespace synthetisens;

extern component* speaker;

Port::Port(PortType type, ComponentEffective* parent) : type(type), parent(parent) {
  this->frame = new Gtk::Frame();
  this->frame->set_size_request(20,20);
  this->frame->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
  this->frame->set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
  this->frame->set_label("");
  this->add(*this->frame);

  this->drag_source_set(std::vector<Gtk::TargetEntry>({Gtk::TargetEntry("create_wire")}), Gdk::BUTTON1_MASK, Gdk::ACTION_COPY);
  this->drag_dest_set(std::vector<Gtk::TargetEntry>({Gtk::TargetEntry("create_wire")}), Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY);

  this->signal_drag_data_get().connect(sigc::mem_fun(*this, &Port::drag_data_get));
  this->signal_drag_data_received().connect(sigc::mem_fun(*this, &Port::drag_data_recvd));
}

void Port::drag_data_get(const Glib::RefPtr<Gdk::DragContext>& context, Gtk::SelectionData& selection_data, guint info, guint time) {
  //Sending the port pointer
  Port* sender = this;
  selection_data.set(selection_data.get_target(), 8, (const guchar*) &sender, sizeof(Port*));
}

void Port::drag_data_recvd(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time) {
  //Receiving the port pointer
  Port** sender_pointer = (Port**)selection_data.get_data();
  Port* sender = *sender_pointer;

  this->link(sender);
  sender->link(this);

  context->drag_finish(true, false, time);
}

void Port::inlink(Port* destination) {
  if(wires.size() > 0){
    wires.erase(wires.begin());
    this->parent->virtual_component->disconnect_input(this->parent->port_position(this));

    if(wires[0]->destination != destination){
      Wire* w = new Wire();
      w->source = this;
      w->destination = destination;

      wires.insert(wires.begin(), 1, w);
      this->parent->virtual_component->connect_input(this->parent->port_position(this), *destination->parent->virtual_component, destination->parent->port_position(destination));
    }
  } else {
    Wire* w = new Wire();
    w->source = this;
    w->destination = destination;

    wires.insert(wires.begin(), 1, w);
    this->parent->virtual_component->connect_input(this->parent->port_position(this), *destination->parent->virtual_component, destination->parent->port_position(destination));
  }
}

int check_existing_link(std::vector<Wire*> links, Port* destination) {
  for(int i = 0; i < links.size(); i++){
    if (links[i]->destination == destination) {return i;}
  }
  return -1;
}

void Port::outlink(Port* destination) {
  int pos = check_existing_link(wires,destination);
  if(pos >= 0){
    wires.erase(wires.begin()+pos);
  } else {
    Wire* w = new Wire();
    w->source = this;
    w->destination = destination;

    wires.insert(wires.begin(), 1, w);
  }
}

void Port::link(Port* destination) {
  const PortType dtype = destination->type;

  if (((type == SIGNAL_INPUT_PORT) && (dtype == SIGNAL_OUTPUT_PORT)) || ((type == VALUE_INPUT_PORT) && (dtype == VALUE_OUTPUT_PORT))) {
    inlink(destination);
  }
  if (((dtype == SIGNAL_INPUT_PORT) && (type == SIGNAL_OUTPUT_PORT)) || ((dtype == VALUE_INPUT_PORT) && (type == VALUE_OUTPUT_PORT))) {
    outlink(destination);
  }
  window->queue_draw();
}

int ComponentEffective::port_position(Port* port) {
  switch (port->type){
    case SIGNAL_INPUT_PORT:
    case VALUE_INPUT_PORT:
      for (int i = 0; i < virtual_component->num_inputs + virtual_component->num_parameters; i++){
        if (input_ports[i] == port) {return i;}
      }
      break;
    case SIGNAL_OUTPUT_PORT:
    case VALUE_OUTPUT_PORT:
      for (int i = 0; i < virtual_component->num_outputs; i++){
        if (output_ports[i] == port) {return i;}
      }
      break;
  }
  return 0;
}

std::vector<Line> Port::draw_wires() {
  std::vector<Line> lines;
  for (int i = 0; i < wires.size(); i++) {
    Line line;
    line.start.x = this->parent->get_allocation().get_x() + this->get_allocation().get_x() + this->get_allocation().get_width()/2;
    line.start.y = this->parent->get_allocation().get_y() + this->get_allocation().get_y() + this->get_allocation().get_height()/2;
    line.end.x = wires[i]->destination->parent->get_allocation().get_x() + wires[i]->destination->get_allocation().get_x() + wires[i]->destination->get_allocation().get_width()/2;
    line.end.y = wires[i]->destination->parent->get_allocation().get_y() + wires[i]->destination->get_allocation().get_y() + wires[i]->destination->get_allocation().get_height()/2;
    lines.push_back(line);
  }
  return lines;
}

ComponentEffective::ComponentEffective(ComponentValue value) : value(value) {
  this->grid = new Gtk::Grid();
  this->box = new Gtk::Box();
  this->img = new Gtk::Image(component_icon[this->value]);
  this->img->set_valign(Gtk::ALIGN_CENTER);
  this->img->set_halign(Gtk::ALIGN_CENTER);
  this->box->add(*(this->img));
  this->add(*grid);

  virtual_component = create_component(value);

  this->signal_button_release_event().connect(sigc::mem_fun(*this, &ComponentEffective::clicked));

  input_ports = new Port*[virtual_component->num_inputs + virtual_component->num_parameters];
  for (int i = 0; i < (virtual_component->num_inputs + virtual_component->num_parameters); i++){
    input_ports[i] = new Port(SIGNAL_INPUT_PORT,this);
  }
  output_ports = new Port*[virtual_component->num_outputs];
  for (int i = 0; i < virtual_component->num_outputs; i++){
    output_ports[i] = new Port(SIGNAL_OUTPUT_PORT,this);
  }

  //MOUNTING THE COMPONENT
  const int num_inputs = virtual_component->num_inputs + virtual_component->num_parameters;
  const int m = MAX(num_inputs, virtual_component->num_outputs);
  if (num_inputs > 0){     //on attache les ports d'entrées à gauche de l'image du composant, les uns aux dessus des autres
    const int h = m/num_inputs;
    grid->attach(*this->input_ports[0],0,0,1,h);
    grid->attach_next_to(*this->box,*this->input_ports[0],Gtk::POS_RIGHT,1,m);
    for(int n = 1; n < num_inputs; n++){
      grid->attach_next_to(*this->input_ports[n],*this->input_ports[n-1],Gtk::POS_BOTTOM,1,h);
    }
  } else {
    grid->attach(*this->box,0,0,1,m);
  }

  if(virtual_component->num_outputs > 0){ //même chose, pour les ports de sortie à droite
    const int h = m/virtual_component->num_outputs;
    grid->attach_next_to(*this->output_ports[0],*this->box,Gtk::POS_RIGHT,1,h);
    for(int n = 1; n < virtual_component->num_outputs; n++){
      grid->attach_next_to(*this->output_ports[n],*this->output_ports[n-1],Gtk::POS_BOTTOM,1,h);
    }
  }
}

std::vector<Line> ComponentEffective::draw_ports() {
  std::vector<Line> lines;
  for (int i = 0; i < virtual_component->num_inputs + virtual_component->num_parameters; i++){
    std::vector<Line> port_lines = input_ports[i]->draw_wires();
    lines.insert(lines.end(), port_lines.begin(), port_lines.end());
  }
  for (int i = 0; i < virtual_component->num_outputs; i++){
    std::vector<Line> port_lines = output_ports[i]->draw_wires();
    lines.insert(lines.end(), port_lines.begin(), port_lines.end());
  }
  return lines;
}

bool ComponentEffective::clicked(GdkEventButton* event) {
  if (this->value == CONSTANT) {
    Gtk::Dialog* dialog = new Gtk::Dialog();
    dialog->set_title("Value");
    dialog->add_button("OK", Gtk::RESPONSE_OK);
    dialog->add_button("Cancel", Gtk::RESPONSE_CANCEL);
    Gtk::Entry* entry = new Gtk::Entry();
    dialog->get_content_area()->add(*entry);
    dialog->show_all();
    int result = dialog->run();
    dialog->close();
    if (result == Gtk::RESPONSE_OK) {
      int n_freq = std::atoi(entry->get_text().data());
      constant_component* vcomponent = (constant_component*) this->virtual_component;
      vcomponent->value = n_freq;
    }
  } else if (this->value == CUSTOM){
    Gtk::FileChooserDialog* dialog = new Gtk::FileChooserDialog("Choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog->set_transient_for(*window);
    dialog->set_modal(true);
    dialog->set_position(Gtk::WIN_POS_CENTER);
    dialog->add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog->add_button("_Open", Gtk::RESPONSE_OK);
    
    int result = dialog->run();
    dialog->close();

    if (result != Gtk::RESPONSE_OK) {
      return false;
    } 

    std::string filename = dialog->get_filename();
    SF_INFO sfinfo;
    SNDFILE* sndfile = sf_open(filename.data(), SFM_READ, &sfinfo);

    if (!sndfile) {
        std::cerr << "Impossible d'ouvrir le fichier : " << filename << std::endl;
    }
    
    const int framecount = sfinfo.frames;
    const int channels = sfinfo.channels;
    const int buffer_size = framecount * channels;
    double* buffer = new double[buffer_size];
    sf_readf_double(sndfile, buffer, buffer_size);
    sf_close(sndfile);

    double* nbuf = new double[framecount];
    for(int i = 0; i < framecount; i++){
      nbuf[i] = buffer[i*channels];
    }

    synthetisens::signal* output_signal = new synthetisens::signal(framecount, nbuf, false);
    synthetisens::signal* noutput_signal = &change_samplerate(*output_signal, sfinfo.samplerate, SAMPLE_FREQ);
    custom_component* ccomponent = (custom_component*) this->virtual_component;

    ccomponent->set_signal(noutput_signal);
  }
  return true;
}

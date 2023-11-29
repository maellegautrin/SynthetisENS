#include "componenteffective.h"
#include "component.h"
#include "component_definition.h"
#include "componentselector.h"
#include "gtkmm/enums.h"
#include "gtkmm/frame.h"
#include "gtkmm/grid.h"
#include "gtkmm/widget.h"
#include "gtkmm/image.h"
#include "gtkmm/box.h"
#include <cstring>
#include <iostream>
#include <list>
#include <string>
#include <vector>



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

extern Port* last_clicked;
extern int port_label;
extern char* label_space;


// void Port::click_handler() {
//   if (!last_clicked) {
//     last_clicked = this;
//   } else {
//     char label_chr = label_space[port_label];
//     char label[2] = {label_chr, '\0'};
//     port_label = (port_label + 1) % strlen(label_space);  
//     this->link(last_clicked,label);
//     last_clicked->link(this,label);
//     last_clicked = NULL;
//   }
// }

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

  this->signal_draw().connect(sigc::mem_fun(*this, &Port::draw_wires));
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
  std::cout << "inlink" << std::endl;
  if(wires.size() > 0){
    wires.erase(wires.begin());
    // this->set_label("");
    this->parent->virtual_component->disconnect_input(this->parent->port_position(this));

    if(wires[0]->destination != destination){
      Wire* w = new Wire();
      w->source = this;
      w->destination = destination;

      wires.insert(wires.begin(), 1, w);
      // const char* coldlabel = this->get_label().c_str();
      // char* oldlabel = strcopy(coldlabel);
      // this->set_label(strcat(oldlabel,label));
      this->parent->virtual_component->connect_input(this->parent->port_position(this), *destination->parent->virtual_component, destination->parent->port_position(destination));
    }
  } else {
    Wire* w = new Wire();
    w->source = this;
    w->destination = destination;

    wires.insert(wires.begin(), 1, w);
    // const char* coldlabel = this->get_label().c_str();
    // char* oldlabel = strcopy(coldlabel);
    // this->set_label(strcat(oldlabel,label));
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
  std::cout << "outlink" << std::endl;
  int pos = check_existing_link(wires,destination);
  if(pos >= 0){
    wires.erase(wires.begin()+pos);
    // const char* coldlabel = this->get_label().c_str();
    // char* oldlabel = strcopy(coldlabel);
    // this->set_label(strremove(oldlabel,label[0]));
  } else {
    Wire* w = new Wire();
    w->source = this;
    w->destination = destination;

    wires.insert(wires.begin(), 1, w);
    // const char* coldlabel = this->get_label().c_str();
    // char* oldlabel = strcopy(coldlabel);
    // this->set_label(strcat(oldlabel,label));
  }
}

void Port::link(Port* destination) {
  const PortType dtype = destination->type;
  std::cout << type << " " << dtype << std::endl;

  if (((type == SIGNAL_INPUT_PORT) && (dtype == SIGNAL_OUTPUT_PORT)) || ((type == VALUE_INPUT_PORT) && (dtype == VALUE_OUTPUT_PORT))) {
    inlink(destination);
  }
  if (((dtype == SIGNAL_INPUT_PORT) && (type == SIGNAL_OUTPUT_PORT)) || ((dtype == VALUE_INPUT_PORT) && (type == VALUE_OUTPUT_PORT))) {
    outlink(destination);
  }
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

void draw_wire(const Cairo::RefPtr<Cairo::Context>& cr, Port* source, Port* destination) {
  std::cout << "drawing wire" << std::endl;
  Gtk::Allocation source_allocation = source->get_allocation();
  Gtk::Allocation destination_allocation = destination->get_allocation();

  const int source_x = source_allocation.get_x() + source_allocation.get_width()/2;
  const int source_y = source_allocation.get_y() + source_allocation.get_height()/2;
  const int destination_x = destination_allocation.get_x() + destination_allocation.get_width()/2;
  const int destination_y = destination_allocation.get_y() + destination_allocation.get_height()/2;

  std::cout << source_x << " " << source_y << " " << destination_x << " " << destination_y << std::endl;

  cr->move_to(source_x, source_y);
  cr->line_to(destination_x, destination_y);
  cr->stroke();
}

bool Port::draw_wires(const Cairo::RefPtr<Cairo::Context>& cr) {
  for (int i = 0; i < wires.size(); i++) {
    Port* source = wires[i]->source;
    Port* destination = wires[i]->destination;

    if (source->type == SIGNAL_OUTPUT_PORT || source->type == VALUE_OUTPUT_PORT) {
      draw_wire(cr, source, destination);
    } else {
      draw_wire(cr, destination, source);
    }
  }

  return true;
}



ComponentEffective::ComponentEffective(ComponentValue value) : value(value) {
  this->box = new Gtk::Box();
  this->img = new Gtk::Image(component_icon[this->value]);
  this->img->set_valign(Gtk::ALIGN_CENTER);
  this->img->set_halign(Gtk::ALIGN_CENTER);
  this->box->add(*(this->img));

  virtual_component = create_component(value);

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
    this->attach(*this->input_ports[0],0,0,1,h);
    this->attach_next_to(*this->box,*this->input_ports[0],Gtk::POS_RIGHT,1,m);
    for(int n = 1; n < num_inputs; n++){
      this->attach_next_to(*this->input_ports[n],*this->input_ports[n-1],Gtk::POS_BOTTOM,1,h);
    }
  } else {
    this->attach(*this->box,0,0,1,m);
  }

  if(virtual_component->num_outputs > 0){ //même chose, pour les ports de sortie à droite
    const int h = m/virtual_component->num_outputs;
    this->attach_next_to(*this->output_ports[0],*this->box,Gtk::POS_RIGHT,1,h);
    for(int n = 1; n < virtual_component->num_outputs; n++){
      this->attach_next_to(*this->output_ports[n],*this->output_ports[n-1],Gtk::POS_BOTTOM,1,h);
    }
  }
}

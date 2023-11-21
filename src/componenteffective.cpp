
#include "component.h"
#include "componenteffective.h"
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

    extern Port* last_clicked;
    extern int port_label;
    extern char* label_space;


component* match_component(ComponentType type, int id);


Wire::Wire(Port* source, Port* destination) : source(source), destination(destination) {


}

void Port::click_handler(){
    if (!last_clicked) {
        last_clicked = this;
    }
    else{
        char label_chr = label_space[port_label];
        char label[2] = {label_chr, '\0'};
        port_label = (port_label + 1) % strlen(label_space);  
        this->link(last_clicked,label);
        last_clicked->link(this,label);
        last_clicked = NULL;
    
}
}

Port::Port(PortType type, ComponentEffective* parent) : type(type), parent(parent) {

    links = std::vector<Wire*>();
    this->set_label("");
    this->signal_clicked().connect(sigc::mem_fun(*this, &Port::click_handler));

}

int check_existing_link(std::vector<Wire*> links, Port* destination) {
    for(int i = 0; i < links.size(); i++){
        if (links[i]->destination == destination) {return i;}
    }
    return -1;
    }

void Port::inlink(Port* destination, char* label){
    if(links.size() > 0){
        links.erase(links.begin());
        this->set_label("");
        this->parent->virtual_component->disconnect_input(this->parent->port_position(this));

        if(links[0]->destination != destination){
            links.insert(links.begin(),1,new Wire(this,destination));
            const char* coldlabel = this->get_label().c_str();
            char* oldlabel = strcopy(coldlabel);
            this->set_label(strcat(oldlabel,label));
            this->parent->virtual_component->connect_input(this->parent->port_position(this), *destination->parent->virtual_component, destination->parent->port_position(destination));
        }
    }
    else{
        links.insert(links.begin(),1,new Wire(this,destination));
        const char* coldlabel = this->get_label().c_str();
        char* oldlabel = strcopy(coldlabel);
        this->set_label(strcat(oldlabel,label));
        this->parent->virtual_component->connect_input(this->parent->port_position(this), *destination->parent->virtual_component, destination->parent->port_position(destination));
    }
}

void Port::outlink(Port* destination, char* label){
    int pos = check_existing_link(links,destination);
            if(pos >= 0){
                links.erase(links.begin()+pos);
                const char* coldlabel = this->get_label().c_str();
                char* oldlabel = strcopy(coldlabel);
                this->set_label(strremove(oldlabel,label[0]));
            }
            else{
                links.insert(links.begin(), 1, new Wire(this,destination));
            const char* coldlabel = this->get_label().c_str();
            char* oldlabel = strcopy(coldlabel);
                this->set_label(strcat(oldlabel,label));
            }
}

void Port::link(Port* destination, char* label) {
        const PortType dtype = destination->type;

        if (((type == SIGNAL_INPUT_PORT) && (dtype == SIGNAL_OUTPUT_PORT)) || ((type == VALUE_INPUT_PORT) && (dtype == VALUE_OUTPUT_PORT))) {
            inlink(destination, label);
        }
        if (((dtype == SIGNAL_INPUT_PORT) && (type == SIGNAL_OUTPUT_PORT)) || ((dtype == VALUE_INPUT_PORT) && (type == VALUE_OUTPUT_PORT))) {
            outlink(destination, label);
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

ComponentEffective::ComponentEffective(char* imglink, ComponentType type, int component_id) : imglink(imglink), type(type), component_id(component_id) {

    this->box = new Gtk::Box();
    this->img = new Gtk::Image(imglink);
    this->img->set_valign(Gtk::ALIGN_CENTER);
    this->img->set_halign(Gtk::ALIGN_CENTER);
    this->box->add(*(this->img));

    virtual_component = match_component(type, component_id);

    input_ports = new Port*[virtual_component->num_inputs + virtual_component->num_parameters];
    for (int i = 0; i < (virtual_component->num_inputs + virtual_component->num_parameters); i++){
        input_ports[i] = new Port(SIGNAL_INPUT_PORT,this);
    }
    output_ports = new Port*[virtual_component->num_outputs];
    for (int i = 0; i < virtual_component->num_outputs; i++){
        output_ports[i] = new Port(SIGNAL_OUTPUT_PORT,this);
    }

}

void ComponentEffective::place(Gtk::Frame* slot){
    const int num_inputs = virtual_component->num_inputs + virtual_component->num_parameters;
    const int m = MAX(num_inputs, virtual_component->num_outputs);
    if (num_inputs > 0){     //on attache les ports d'entrées à gauche de l'image du composant, les uns aux dessus des autres
        const int h = m/num_inputs;
        this->attach(*this->input_ports[0],0,0,1,h);
        this->attach_next_to(*this->box,*this->input_ports[0],Gtk::POS_RIGHT,1,m);
        for(int n = 1; n < num_inputs; n++){
            this->attach_next_to(*this->input_ports[n],*this->input_ports[n-1],Gtk::POS_BOTTOM,1,h);
        }
    }
    else{
        this->attach(*this->box,0,0,1,m);
    }

    if(virtual_component->num_outputs > 0){ //même chose, pour les ports de sortie à droite
        const int h = m/virtual_component->num_outputs;
        this->attach_next_to(*this->output_ports[0],*this->box,Gtk::POS_RIGHT,1,h);
        for(int n = 1; n < virtual_component->num_outputs; n++){
            this->attach_next_to(*this->output_ports[n],*this->output_ports[n-1],Gtk::POS_BOTTOM,1,h);
        }
    }

    slot->remove();
    slot->add(*this);

}






component* match_component(ComponentType type, int id){
    if (type == SIGNAL_COMPONENT){
        if(id == 0) {return (new sinusoidal_component());}
        if(id == 1) {return (new square_component());}
        if(id == 2) {return (new triangle_component());}
        if(id == 3) {return (new sawtooth_component());}
    }
    if (type == FILTER_COMPONENT){
    }
    if (type == OPERATOR_COMPONENT){
        if(id == 0) {return (new sum_component());}
        if(id == 1) {return (new sub_component());}
        if(id == 2) {return (new prod_component());}
        if(id == 3) {return (new div_component());}
        if(id == 4) {return (new derivative_component());}
        if(id == 5) {return (new primitive_component());}
        if(id == 6) {return (new normalize_component());}

    }
    if (type == OTHER_COMPONENT){
        if(id == 0) {return (new speaker_component());}
        if(id == 1) {return (new constant_component(0.0));}
        if(id == 2) {return (new constant_component(0.0));}
        if(id == 3) {return (new constant_component(0.0));}
        if(id == 4) {return (new constant_component(0.0));}
        if(id == 5) {return (new constant_component(1.0));}
        if(id == 6) {return (new constant_component(440.0));}
        if(id == 7) {return (new constant_component(880.0));}
    }
    return new constant_component(0.0);

}

void test(char* text){ std::cout << text;}


#include "component.h"
#include "componenteffective.h"
#include "componentselector.h"
#include "gtkmm/enums.h"
#include "gtkmm/grid.h"
#include "gtkmm/widget.h"
#include "gtkmm/image.h"
#include "gtkmm/box.h"
#include <iostream>
#include <list>
#include <vector>


using namespace synthetisens;

Wire::Wire(Port* source, Port* destination) : source(source), destination(destination) {
}

Port::Port(PortType type) : type(type) {

    links = std::vector<Wire*>();

}

int check_existing_link(std::vector<Wire*> links, Port* destination) {
    for(int i = 0; i < links.size(); i++){
        if (links[i]->destination == destination) {return i;}
    }
    return -1;
    }

void Port::inlink(Port* destination){
    if(links.size() > 0){
        if(links[0]->destination == destination){
            links.erase(links.begin());
        }
    }
    else{
        links.insert(links.begin(),1,new Wire(this,destination));
    }
}

void Port::outlink(Port* destination){
    int pos = check_existing_link(links,destination);
            if(pos >= 0){
                links.erase(links.begin()+pos);
            }
            else{
                links.insert(links.begin(), 1, new Wire(this,destination));
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
    };

ComponentEffective::ComponentEffective(char* imglink, ComponentType type, int component_id) : imglink(imglink), type(type), component_id(component_id) {

    this->box = new Gtk::Box();
    this->img = new Gtk::Image(imglink);
    this->img->set_valign(Gtk::ALIGN_CENTER);
    this->img->set_halign(Gtk::ALIGN_CENTER);
    this->box->add(*(this->img));

    virtual_component = match_component(type, component_id);


    input_ports = new Port*[virtual_component->num_inputs];
    for (int i = 0; i < virtual_component->num_inputs; i++){
        input_ports[i] = new Port(SIGNAL_INPUT_PORT);
    }
    output_ports = new Port*[virtual_component->num_outputs];
    for (int i = 0; i < virtual_component->num_outputs; i++){
        output_ports[i] = new Port(SIGNAL_OUTPUT_PORT);
    }

}

void ComponentEffective::place(Gtk::Container* slot){
    const int m = MAX(virtual_component->num_inputs, virtual_component->num_outputs);
    if (virtual_component->num_inputs > 0){     //on attache les ports d'entrées à gauche de l'image du composant, les uns aux dessus des autres
        const int h = m/virtual_component->num_inputs;
        this->attach(*this->input_ports[0],0,0,1,h);
        this->attach_next_to(*this->box,*this->input_ports[0],Gtk::POS_RIGHT,1,m);
        for(int n = 1; n < virtual_component->num_inputs; n++){
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

    slot->add(*this);

}


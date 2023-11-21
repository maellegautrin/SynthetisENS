
#include "component.h"
#include "componenteffective.h"
#include "componentselector.h"
#include "gtkmm/widget.h"
#include "gtkmm/image.h"
#include "gtkmm/box.h"
#include <iostream>
#include <list>
#include <vector>


using namespace synthetisens;

Wire::Wire(Port* source, Port* destination) : source(source), destination(destination) {
}

Port::Port(PortType type, ComponentEffective* parent) : type(type), parent(parent) {

    links = std::vector<Wire*>();

}

int check_existing_link(std::vector<Wire*> links, Port* destination) {
    for(int i = 0; i < links.size(); i++){
        if (links[i]->destination == destination) {return i;}
    }
    return -1;
    }

void Port::inlink(Port* destination){
    if(links.size() == 0){
        links.insert(links.begin(),1,new Wire(this,destination));
    }
    else{
        links.erase(links.begin());
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
            destination->outlink(this);
        }
        if (((dtype == SIGNAL_INPUT_PORT) && (type == SIGNAL_OUTPUT_PORT)) || ((dtype == VALUE_INPUT_PORT) && (type == VALUE_OUTPUT_PORT))) {
            outlink(destination);
            destination->inlink(this);
        }
    };

ComponentEffective::ComponentEffective(char* imglink, ComponentType type, int component_id) : imglink(imglink), type(type), component_id(component_id) {

    this->img = new Gtk::Image(imglink);
    this->img->set_valign(Gtk::ALIGN_CENTER);
    this->img->set_halign(Gtk::ALIGN_CENTER);
    this->add(*(this->img)); 
    this->set_size_request(125,125);

    virtual_component = new sinusoidal_component();

    input_ports = new Port*[virtual_component->num_inputs];
    output_ports = new Port*[virtual_component->num_outputs];

}
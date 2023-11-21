#ifndef _COMPONENT_EFFECTIVE_H
#define _COMPONENT_EFFECTIVE_H



#include "component.h"
#include "componentselector.h"
#include "gtkmm/widget.h"
#include "gtkmm/image.h"
#include "gtkmm/box.h"
#include <list>
#include <vector>




namespace synthetisens{

typedef enum {
    SIGNAL_INPUT_PORT,
    SIGNAL_OUTPUT_PORT,
    VALUE_INPUT_PORT,
    VALUE_OUTPUT_PORT
} PortType;


typedef class Port Port;
typedef class ComponentEffective ComponentEffective;

class Wire : public Gtk::Widget {
    public:

    Wire(Port* source, Port* destination);
    Port* source;
    Port* destination;

};

class Port : public Gtk::Image {
    public:
    
    Port(PortType type, ComponentEffective* parent);

    const PortType type;    
    std::vector<Wire*> links;
    void link(Port* destination);
    void outlink(Port* destination);
    void inlink(Port* destination);

    private:

    ComponentEffective* parent;
    Wire* wire(Port* destination);
};

class ComponentEffective : public Gtk::Box {
    public:

    ComponentEffective(char* img, ComponentType type, int component_id);

    const int component_id;
    const ComponentType type;
    char* imglink;


    private:

    Gtk::Image* img;
    component* virtual_component;
    Port** input_ports;
    Port** output_ports;


};

};

#endif
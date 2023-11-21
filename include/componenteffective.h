#ifndef _COMPONENT_EFFECTIVE_H
#define _COMPONENT_EFFECTIVE_H



#include "component.h"
#include "componentselector.h"
#include "gtkmm/grid.h"
#include "gtkmm/frame.h"
#include "gtkmm/button.h"
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

class Port : public Gtk::Button {
    public:
    
    Port(PortType type, ComponentEffective* parent);
    void click_handler();

    const PortType type;
    std::vector<Wire*> links;
    void link(Port* destination, char* label);
    void outlink(Port* destination, char* label);
    void inlink(Port* destination, char* label);

    private:

    ComponentEffective* parent;

};

class ComponentEffective : public Gtk::Grid {
    public:

    ComponentEffective(char* img, ComponentType type, int component_id);

    void place(Gtk::Frame* slot);

    const int component_id;
    const ComponentType type;
    component* virtual_component;
    char* imglink;

    int port_position(Port* port);

    private:

    Gtk::Box* box;
    Gtk::Image* img;
    Port** input_ports;
    Port** output_ports;


};


};





#endif

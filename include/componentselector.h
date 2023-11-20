#ifndef _COMPONENT_SELECTOR_H
#define _COMPONENT_SELECTOR_H


#include "component.h"
#include "gtkmm/box.h"
#include "gtkmm/image.h"



namespace synthetisens {

typedef enum {
    SIGNAL_COMPONENT,
    FILTER_COMPONENT,
    OPERATOR_COMPONENT,
    OTHER_COMPONENT} ComponentType;

class ComponentSelector : public Gtk::Box {
    public:
        ComponentSelector(char* imglink, ComponentType type, int component_id);

        const int component_id;
        const ComponentType type;
        char* imglink;

    private:

        Gtk::Image* img;
    
};


};


#endif
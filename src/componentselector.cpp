#include "componentselector.h"
#include "gtkmm/enums.h"
#include "gtkmm/image.h"
#include "gtkmm/box.h"

using namespace synthetisens;

ComponentSelector::ComponentSelector(char* imglink, ComponentType type, int component_id) : imglink(imglink), type(type), component_id(component_id) {
    this->img = new Gtk::Image(imglink);
    this->img->set_valign(Gtk::ALIGN_CENTER);
    this->img->set_halign(Gtk::ALIGN_CENTER);
    this->add(*(this->img)); 
    this->set_size_request(125,125);
}
#include "componentselector.h"
#include "gtkmm/enums.h"
#include "gtkmm/image.h"
#include "gtkmm/box.h"
#include <iostream>

using namespace synthetisens;

extern ComponentSelector* current_selector;

void ComponentSelector::click_handler(){
  current_selector = this;
}

ComponentSelector::ComponentSelector(char* imglink, ComponentType type, int component_id) : imglink(imglink), type(type), component_id(component_id) {
  this->set_size_request(125,125);
  this->img = new Gtk::Image(imglink);
  this->img->set_valign(Gtk::ALIGN_CENTER);
  this->img->set_halign(Gtk::ALIGN_CENTER);
  this->set_image(*(this->img)); 

  this->signal_clicked().connect(sigc::mem_fun(*this, &ComponentSelector::click_handler));
}

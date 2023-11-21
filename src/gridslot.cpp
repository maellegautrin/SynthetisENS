#include "gridslot.h"
#include "componentselector.h"
#include "gtkmm/button.h"
#include "gtkmm/grid.h"
#include <iostream>


using namespace synthetisens;

extern ComponentSelector* current_selector;

void GridSlot::click_handler(){
  if(current_selector != NULL){
    std::cout << "caca";
    this->remove();
    this->add(*current_selector);
    current_selector = NULL;
  }
}

GridSlot::GridSlot(Gtk::Grid* grid, int i, int j){
  this->set_size_request(150,100);
  grid->attach(*this,i,j,1,1);
  slot_button = new Gtk::Button();
  this->add(*slot_button);

  slot_button->signal_clicked().connect(sigc::mem_fun(*this, &GridSlot::click_handler));
}
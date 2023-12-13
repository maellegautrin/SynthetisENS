#include "select_panel.h"
#include "component_definition.h"

#include <iostream>

using namespace std;

using namespace synthetisens;

Tab::Tab(ComponentType tabType) : tabType(tabType) {
  grid = Gtk::manage(new Gtk::Grid());

  selectors = std::vector<ComponentSelector*>();
  
  int i = 0;
  for (ComponentValue value = ComponentValue(0); value < COMPONENT_VALUE_COUNT; value = ComponentValue(value + 1)) {
    if (component_type[value] == tabType) {
      ComponentSelector* selector = Gtk::manage(new ComponentSelector(value));
      selectors.push_back(selector);
      grid->attach(*selector, i % 3, i / 3, 1, 1);
      i++;
    }
  }

  add(*grid);
}


SelectPanel::SelectPanel() {
  notebook = Gtk::manage(new Gtk::Notebook());
  notebook->set_size_request(375, 400);

  tabs = std::vector<Tab*>();

  for (ComponentType type = ComponentType(0); type < COMPONENT_TYPE_COUNT; type = ComponentType(type + 1)) {
    Tab* tab = Gtk::manage(new Tab(type));
    tabs.push_back(tab);
    notebook->append_page(*tab, component_type_name[type]);
  }

  add(*notebook);
}

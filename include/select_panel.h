#ifndef _SELECT_PANEL_H_
#define _SELECT_PANEL_H_

#include "component_definition.h"
#include "componentselector.h"
#include "gtkmm/frame.h"
#include "gtkmm/notebook.h"
#include "gtkmm/scrolledwindow.h"
#include "gtkmm/grid.h"
#include <vector>

namespace synthetisens {

  
  class Tab : public Gtk::ScrolledWindow {
    private:
      Gtk::Grid* grid;

      std::vector<ComponentSelector*> selectors;

    public:
      const ComponentType tabType;

      Tab(ComponentType tabType);
  };

  
  //============================================================================

  class SelectPanel : public Gtk::Frame {
    private:
      Gtk::Notebook* notebook;

      std::vector<Tab*> tabs;

    public:
      SelectPanel();
  };

}

#endif // !_SELECT_PANEL_H_

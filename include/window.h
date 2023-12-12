#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "gtkmm/grid.h"
#include "gtkmm/window.h"
#include "menubar.h"
#include "iconbar.h"
#include "select_panel.h"
#include "workspace_panel.h"

namespace synthetisens {

  class Window : public Gtk::Window {
    private:
      Gtk::Grid* grid;

      MenuBar* menuBar;
      IconBar* iconBar;
      SelectPanel* selectPanel;
      WorkspacePanel* workspacePanel;

    public:
      Window();
  };

}

#endif // !_WINDOW_H_

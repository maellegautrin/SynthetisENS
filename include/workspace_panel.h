#ifndef _WORKSPACE_PANEL_H_
#define _WORKSPACE_PANEL_H_

#include "circuit_area.h"
#include "gtkmm/frame.h"
#include "gtkmm/notebook.h"
#include "keyboard.h"

namespace synthetisens {

  class WorkspacePanel : public Gtk::Frame {
    private:
      Gtk::Notebook* notebook;

      CircuitArea* circuitArea;
      Keyboard* keyboard;

    public:
      WorkspacePanel();
  };

}

#endif // !DEBUG

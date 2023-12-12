#ifndef _WORKSPACE_PANEL_H_
#define _WORKSPACE_PANEL_H_

#include "circuit_area.h"
#include "gtkmm/frame.h"
#include "gtkmm/notebook.h"

namespace synthetisens {

  class WorkspacePanel : public Gtk::Frame {
    private:
      Gtk::Notebook* notebook;

      CircuitArea* circuitArea;

    public:
      WorkspacePanel();
  };

}

#endif // !DEBUG

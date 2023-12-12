#ifndef _WORKSPACE_PANEL_H_
#define _WORKSPACE_PANEL_H_

#include "circuit_area.h"
#include "gtkmm/frame.h"
#include "gtkmm/notebook.h"
#include "sequencer.h"

namespace synthetisens {

  class WorkspacePanel : public Gtk::Frame {
    private:
      Gtk::Notebook* notebook;

      CircuitArea* circuitArea;
      Sequencer* sequencer;

    public:
      WorkspacePanel();
  };

}

#endif // !DEBUG

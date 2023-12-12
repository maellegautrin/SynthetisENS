#include "workspace_panel.h"
#include "gtkmm/object.h"

using namespace synthetisens;

WorkspacePanel::WorkspacePanel() {
  notebook = Gtk::manage(new Gtk::Notebook());

  circuitArea = Gtk::manage(new CircuitArea());
  circuitArea->set_size_request(900, 600);
  
  sequencer = Gtk::manage(new Sequencer(12,15));
  sequencer->set_size_request(900,600);

  notebook->append_page(*circuitArea, "Circuit");
  notebook->append_page(*sequencer, "Sequencer");

  add(*notebook);
}

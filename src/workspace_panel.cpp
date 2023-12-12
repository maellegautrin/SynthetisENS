#include "workspace_panel.h"
#include "keyboard.h"

using namespace synthetisens;

WorkspacePanel::WorkspacePanel() {
  notebook = Gtk::manage(new Gtk::Notebook());

  circuitArea = Gtk::manage(new CircuitArea());
  circuitArea->set_size_request(900, 600);

  keyboard = Gtk::manage(new Keyboard());
  keyboard->set_size_request(900, 600);

  notebook->append_page(*circuitArea, "Circuit");
  notebook->append_page(*keyboard, "Keyboard");

  add(*notebook);
}

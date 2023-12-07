#include "window.h"
#include "gtkmm/enums.h"

using namespace synthetisens;

Window::Window() {
  set_title("synthetisENS");
  set_default_size(1500, 1000);
  set_position(Gtk::WIN_POS_CENTER);

  grid = Gtk::manage(new Gtk::Grid());
  menuBar = Gtk::manage(new MenuBar());
  selectPanel = Gtk::manage(new SelectPanel());
  workspacePanel = Gtk::manage(new WorkspacePanel());

  grid->attach(*menuBar, 0, 0, 3, 1);
  grid->attach_next_to(*selectPanel, *menuBar, Gtk::POS_BOTTOM, 3, 3);
  grid->attach_next_to(*workspacePanel, *selectPanel, Gtk::POS_RIGHT, 3, 3);
  
  add(*grid);

  show_all_children();
}

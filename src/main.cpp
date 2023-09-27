#include <gtkmm.h>

int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "synthetisens.app");

  Gtk::Window window;
  window.set_default_size(200, 200);
  window.set_title("synthetisENS");

  return app->run(window);
}

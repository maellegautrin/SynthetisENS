// Forcing OS to linux (useful for Stk)
#define __OS_LINUX__

#include <iostream>
#include <sndfile.h>

#include "window.h"
#include "component.h"
#include "gtkmm/application.h"
#include <Stk.h>

using namespace std;

synthetisens::Window* window;
synthetisens::speaker_component* speaker;
synthetisens::keyboard_component* keyboard;
int duration = 10 * SAMPLE_FREQ;
bool playing = false;

// Main function
int main(int argc, char *argv[])
{
  std::cout << stk::Stk::sampleRate() << std::endl;
  auto app = Gtk::Application::create(argc, argv, "synthetisens.app");

  window = new synthetisens::Window();
  speaker = new synthetisens::speaker_component();

    window->signal_key_press_event().connect(sigc::mem_fun(*(window->workspacePanel->keyboard), &synthetisens::Keyboard::press));
  window->signal_key_release_event().connect(sigc::mem_fun(*(window->workspacePanel->keyboard), &synthetisens::Keyboard::release));

  stk::Stk::setSampleRate(44100.0);
  stk::Stk::showWarnings(true);

  return app->run(*window);
}

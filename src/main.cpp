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
synthetisens::component* speaker;
int duration = 10 * SAMPLE_FREQ;
bool playing = false;

// Main function
int main(int argc, char *argv[])
{
  std::cout << stk::Stk::sampleRate() << std::endl;
  auto app = Gtk::Application::create(argc, argv, "synthetisens.app");

  window = new synthetisens::Window();
  speaker = new synthetisens::speaker_component();

  stk::Stk::setSampleRate(44100.0);
  stk::Stk::showWarnings(true);

  return app->run(*window);
}
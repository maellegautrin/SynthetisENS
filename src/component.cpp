#include "component.h"

using namespace synthetisens;

component::component(int num_inputs, int num_outputs) {
  this->num_inputs = num_inputs;
  this->num_outputs = num_outputs;

  this->inputs = new component*[num_inputs];
  this->outputs = new std::vector<component*>[num_outputs];
}

component::~component() {
  delete[] this->inputs;
  delete[] this->outputs;
}

signal** component::get_input_signals() {
  signal** signals = new signal*[this->num_inputs];
  
  for (int i = 0; i < this->num_inputs; i++) {
    signals[i] = &this->inputs[i]->get_output_signal(this);
  }

  return signals;
}

signal& component::get_output_signal(component* output) {
  for (int i = 0; i < this->num_outputs; i++) {
    for (component* comp : this->outputs[i]) {
      if (comp == output) return generate_signal(i);
    }
  }

  return generate_signal(0);
}

sinusoidal_component::sinusoidal_component(double frequency) : component(0, 1), frequency(frequency) {}

signal& sinusoidal_component::generate_signal(int output) {
sin_signal* output_signal = new sin_signal(frequency);
return *output_signal;
}

sum_component::sum_component(component& c1, component& c2) : component(2,1) {
inputs[0]=&c1;
inputs[1]=&c2;
}

signal& sum_component::generate_signal(int output) {
  signal** inputs = get_input_signals();
  signal& output_signal = *inputs[0] + *inputs[1];
  return output_signal;
}
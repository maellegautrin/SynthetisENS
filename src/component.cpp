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

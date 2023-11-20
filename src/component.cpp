#include "component.h"
#include <cmath>

using namespace synthetisens;

#define PI_2 6.283185307179586476925286766559

component::component(int num_inputs, int num_parameters, int num_outputs) : num_inputs(num_inputs), num_parameters(num_parameters), num_outputs(num_outputs) {
  this->inputs = new component*[num_inputs];
  this->parameters = new double[num_parameters];
  this->outputs = new std::vector<component*>[num_outputs];
}

component::~component() {
  delete[] this->inputs;
  delete[] this->parameters;
  delete[] this->outputs;
}

signal** component::get_input_signals() {
  signal** signals = new signal*[this->num_inputs];
  
  for (int i = 0; i < this->num_inputs; i++) {
    if (this->inputs[i] == nullptr) signals[i] = new signal();
    else signals[i] = &this->inputs[i]->get_output_signal(this);
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

void component::connect_input(component& input, int input_num) {
  if (input_num < 0 || input_num >= this->num_inputs) return;
  this->inputs[input_num] = &input;
}

void component::disconnect_input(int input_num) {
  if (input_num < 0 || input_num >= this->num_inputs) return;
  this->inputs[input_num] = nullptr;
}

void component::connect_output(component& output, int output_num) {
  if (output_num < 0 || output_num >= this->num_outputs) return;
  this->outputs[output_num].push_back(&output);
}

void component::disconnect_output(component& output, int output_num) {
  if (output_num < 0 || output_num >= this->num_outputs) return;
  for (int i = 0; i < this->outputs[output_num].size(); i++) {
    if (this->outputs[output_num][i] == &output) {
      this->outputs[output_num].erase(this->outputs[output_num].begin() + i);
      return;
    }
  }
}

void component::set_parameter(int parameter_num, double value) {
  if (parameter_num < 0 || parameter_num >= this->num_parameters) return;
  this->parameters[parameter_num] = value;
}


sinusoidal_component::sinusoidal_component(double frequency, double amplitude, double phase) : component(0, 3, 1) {
  this->parameters[0] = frequency;
  this->parameters[1] = amplitude;
  this->parameters[2] = phase;
}

signal& sinusoidal_component::generate_signal(int output) {
  int size = SAMPLE_FREQ / this->parameters[0];
  double* values = new double[size];
  for (int i = 0; i < size; i++) {
    values[i] = this->parameters[1] * sin(PI_2 * this->parameters[0] * i / SAMPLE_FREQ + this->parameters[2]);
  }
  signal* output_signal = new signal(size, values, true);
  return *output_signal;
}

square_component::square_component(double frequency, double max_amplitude, double min_amplitude) : component(0, 3, 1) {
  this->parameters[0] = frequency;
  this->parameters[1] = max_amplitude;
  this->parameters[2] = min_amplitude;
}

signal& square_component::generate_signal(int output) {
  int size = SAMPLE_FREQ / this->parameters[0];
  double* values = new double[size];
  for (int i = 0; i < (size/2); i++) {
      values[i] = this->parameters[2];
  }
  for (int i = size/2; i < size; i++) {
      values[i] = this->parameters[1];
  }
  signal* output_signal = new signal(size, values, true);
  return *output_signal;
}

triangle_component::triangle_component(double frequency, double amplitude) : component(0, 2, 1) {
  this->parameters[0] = frequency;
  this->parameters[1] = amplitude;
}

signal& triangle_component::generate_signal(int output) {
  int size = SAMPLE_FREQ / this->parameters[0];
  double* values = new double[size];
  for (int i = 0; i < (size/2); i++) {
      values[i] = (this->parameters[1]*i*2)/size;
  }
  for (int i = size/2; i < size; i++) {
      values[i] = this->parameters[1]*(1-2*i/size);
  }
  signal* output_signal = new signal(size, values, true);
  return *output_signal;
}

sawtooth_component::sawtooth_component(double frequency, double amplitude) : component(0, 2, 1) {
  this->parameters[0] = frequency;
  this->parameters[1] = amplitude;
}

signal& sawtooth_component::generate_signal(int output) {
  int size = SAMPLE_FREQ / this->parameters[0];
  double* values = new double[size];
  for (int i = 0; i < size; i++) {
      values[i] = (this->parameters[1]*i)/size;
  }
  signal* output_signal = new signal(size, values, true);
  return *output_signal;
}
sum_component::sum_component(component& c1, component& c2) : component(2, 0, 1) {
  inputs[0]=&c1;
  inputs[1]=&c2;
}

signal& sum_component::generate_signal(int output) {
  signal** inputs = get_input_signals();
  signal& output_signal = *inputs[0] + *inputs[1];
  return output_signal;
}

prod_component::prod_component(component& c1, component& c2) : component(2, 0, 1) {
  inputs[0]=&c1;
  inputs[1]=&c2;
}

signal& prod_component::generate_signal(int output) {
  signal** inputs = get_input_signals();
  signal& output_signal = *inputs[0] * *inputs[1];
  return output_signal;
}

sub_component::sub_component(component& c1, component& c2) : component(2, 0, 1) {
  inputs[0]=&c1;
  inputs[1]=&c2;
}

signal& sub_component::generate_signal(int output) {
  signal** inputs = get_input_signals();
  signal& output_signal = *inputs[0] - *inputs[1];
  return output_signal;
}

div_component::div_component(component& c1, component& c2) : component(2, 0, 1) {
  inputs[0]=&c1;
  inputs[1]=&c2;
}

signal& div_component::generate_signal(int output) {
  signal** inputs = get_input_signals();
  signal& output_signal = *inputs[0] / *inputs[1];
  return output_signal;
}

derivative_component::derivative_component(component& c1) : component(1, 0, 1) {
  inputs[0]=&c1;
}

signal& derivative_component::generate_signal(int output) {
  signal** inputs = get_input_signals();
  signal& output_signal = derivative(*inputs[0]);
  return output_signal;
}

normalize_component::normalize_component(component& c1) : component(1, 0, 1) {
  inputs[0]=&c1;
}

signal& normalize_component::generate_signal(int output) {
  signal** inputs = get_input_signals();
  signal& output_signal = normalize(*inputs[0]);
  return output_signal;
}

primitive_component::primitive_component(component& c1) : component(1, 0, 1) {
  inputs[0]=&c1;
}

signal& primitive_component::generate_signal(int output) {
  signal** inputs = get_input_signals();
  signal& output_signal = primitive(*inputs[0]);
  return output_signal;
}
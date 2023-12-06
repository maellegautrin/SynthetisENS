#define _USE_MATH_DEFINES

#include "component.h"
#include <cmath>

using namespace synthetisens;

#define PI_2 6.283185307179586476925286766559

component::component(int num_inputs, int num_parameters, int num_outputs) : num_inputs(num_inputs), num_parameters(num_parameters), num_outputs(num_outputs) {
  this->inputs = new input_port[num_inputs + num_parameters];

  for(int i = 0; i < num_inputs; i++) {
    this->inputs[i] = {nullptr, -1};
  }
  for(int i = num_inputs; i < num_inputs + num_parameters; i++) {
    this->inputs[i] = {nullptr, -1};
  }
}

component::~component() {
  delete[] this->inputs;
}

signal** component::get_input_signals() {
  signal** signals = new signal*[this->num_inputs];
  
  for (int i = 0; i < this->num_inputs; i++) {
    if (this->inputs[i].component == nullptr) signals[i] = new signal();
    else signals[i] = this->inputs[i].component->generate_output(this->inputs[i].port_sender).value.signal;
  }

  return signals;
}

double* component::get_parameters() {
  double* parameters = new double[this->num_parameters];

  for (int i = 0; i < this->num_parameters; i++) {
    if (this->inputs[i + this->num_inputs].component == nullptr) parameters[i] = 0;
    else parameters[i] = this->inputs[i + this->num_inputs].component->generate_output(this->inputs[i + this->num_inputs].port_sender).value.parameter;
  }

  return parameters;
}

void component::connect_input(int input_num, component& input, int port_sender) {
  if (input_num < 0 || input_num >= this->num_inputs + this->num_parameters) return;
  this->inputs[input_num] = {&input, port_sender};
}

void component::disconnect_input(int input_num) {
  if (input_num < 0 || input_num >= this->num_inputs + this->num_parameters) return;
  this->inputs[input_num] = {nullptr, -1};
}

port_type component::get_port_type(int port_num) const {
  if (port_num < 0 || port_num >= this->num_inputs + this->num_parameters) return SIGNAL;
  if (port_num < this->num_inputs) return SIGNAL;
  return VALUE;
}

speaker_component::speaker_component() : component(1, 0, 0) {}

output_value& speaker_component::generate_output(int output) {
  signal** inputs = get_input_signals();
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = inputs[0];
  return *output_value;
}

sinusoidal_component::sinusoidal_component() : component(0, 3, 1) {}

output_value& sinusoidal_component::generate_output(int output) {
  double* parameters = this->get_parameters();
  int size = SAMPLE_FREQ / parameters[0];
  double* values = new double[size];
  for (int i = 0; i < size; i++) {
    values[i] = parameters[1] * sin(2 * M_PI * parameters[0] * i / SAMPLE_FREQ + parameters[2]);
  }
  signal* output_signal = new signal(size, values, true);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = output_signal;
  return *output_value;
}

square_component::square_component() : component(0, 4, 1) {}

output_value& square_component::generate_output(int output) {
  double* parameters = this->get_parameters();
  int size = SAMPLE_FREQ / parameters[0];
  double* values = new double[size];
  if (1 - parameters[4] / PI_2 * size > (double)size / 2) {
    for (int i=0; i< 1 - parameters[4] / PI_2 * size; i++ ){
      values[i] = parameters[1]; //max
    }
    for (int i = 1- parameters[4] / PI_2 * size; i < ((double)size / 2 + 1 - parameters[4] / PI_2 * size); i++) {
        values[i] = parameters[2]; // min
    }
    for (int i = size/2; i < size; i++) {
        values[i] = parameters[1]; // max
    }
  } else {
    for (int i=0; i < (double)size / 2 - parameters[4] / PI_2 * size; i++ ){
      values[i] = parameters[2]; //min
    }
    for (int i = (double)size / 2 - parameters[4] / PI_2 * size; i < ((double)size / 2 - parameters[4] / PI_2 * size); i++) {
        values[i] = parameters[1]; // max
    }
    for (int i = (double)size / 2 - parameters[4] / PI_2 * size; i < size; i++) {
        values[i] = parameters[1]; // min
    }
  }
  signal* output_signal = new signal(size, values, true);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = output_signal;
  return *output_value;
}

triangle_component::triangle_component() : component(0, 3, 1) {}

output_value& triangle_component::generate_output(int output) {
  double* parameters = this->get_parameters();
  int size = SAMPLE_FREQ / parameters[0];
  double* values = new double[size];
   if (1 - parameters[4] / PI_2 * size > (double)size / 2){
    for (int i=0; i< 1 - parameters[4] / PI_2 * size; i++ ){
      values[i] = ((-2 * parameters[1]) / size) * i + (2 * parameters[2]) * (1 - parameters[2]/ PI_2); //descendante
    }
    for (int i = 1 - parameters[4] / PI_2 * size; i < ((double)size / 2 + 1 - parameters[4] / PI_2*size); i++) {
        values[i] = ((-2 * parameters[1]) / size) * i + (2 * parameters[1]) * (parameters[2] / PI_2 - 1) ; // montante
    }
    for (int i = size/2; i < size; i++) {
        values[i] = (-2 * parameters[1]) / size * i + (2 * parameters[1]) * (0.5 - parameters[2] / PI_2) + parameters[1]; // descendante
    }
  } else {
    for (int i=0; i < (double)size / 2 - parameters[4] / PI_2 * size; i++ ){
      values[i] = ((2 * parameters[1]) / size) * (parameters[2] / PI_2 * size + i); //montante
    }
    for (int i = (double)size / 2 - parameters[4] / PI_2 * size; i < ((double)size / 2 - parameters[4] / PI_2 * size); i++) {
        values[i] = (-2 * parameters[1] / size) * i + (2 * parameters[1]) * (1 - parameters[2] / PI_2); // descendante
    }
    for (int i = (double)size / 2 - parameters[4] / PI_2 * size; i < size; i++) {
        values[i] = (2 * parameters[1] / size) * i + (-2 * parameters[1]) * (1 - parameters[2] / PI_2); // montante
    }
  }
  signal* output_signal = new signal(size, values, true);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = output_signal;
  return *output_value;
}

sawtooth_component::sawtooth_component() : component(0, 2, 1) {}

output_value& sawtooth_component::generate_output(int output) {
  double* parameters = get_parameters();
  int size = SAMPLE_FREQ / parameters[0];
  double* values = new double[size];
  for (int i = 0; i < size; i++) {
      values[i] = (parameters[1]*i)/size;
  }
  signal* output_signal = new signal(size, values, true);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = output_signal;
  return *output_value;
}

sum_component::sum_component() : component(2, 0, 1) {}

output_value& sum_component::generate_output(int output) {
  signal** inputs = get_input_signals();
  signal& output_signal = *inputs[0] + *inputs[1];
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = &output_signal;
  return *output_value;
}

sub_component::sub_component() : component(2, 0, 1) {}

output_value& sub_component::generate_output(int output) {
  signal** inputs = get_input_signals();
  signal& output_signal = *inputs[0] - *inputs[1];
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = &output_signal;
  return *output_value;
}

prod_component::prod_component() : component(2, 0, 1) {}

output_value& prod_component::generate_output(int output) {
  signal** inputs = get_input_signals();
  signal& output_signal = *inputs[0] * *inputs[1];
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = &output_signal;
  return *output_value;
}
div_component::div_component() : component(2, 0, 1) {}

output_value& div_component::generate_output(int output) {
  signal** inputs = get_input_signals();
  signal& output_signal = *inputs[0] / *inputs[1];
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = &output_signal;
  return *output_value;
}

derivative_component::derivative_component() : component(1, 0, 1) {}

output_value& derivative_component::generate_output(int output) {
  signal** inputs = get_input_signals();
  signal& output_signal = derivative(*inputs[0]);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = &output_signal;
  return *output_value;
}

normalize_component::normalize_component() : component(1, 0, 1) {}

output_value& normalize_component::generate_output(int output) {
  signal** inputs = get_input_signals();
  signal& output_signal = normalize(*inputs[0]);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = &output_signal;
  return *output_value;
}

primitive_component::primitive_component() : component(1, 0, 1) {}

output_value& primitive_component::generate_output(int output) {
  signal** inputs = get_input_signals();
  signal& output_signal = primitive(*inputs[0]);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = &output_signal;
  return *output_value;
}

custom_component::custom_component() : component(0,0,1), sig(new signal()) {}

void custom_component::set_signal(synthetisens::signal* sig){
  this->sig = sig;
}

output_value& custom_component::generate_output(int output) {
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = this->sig;
  return *output_value;

}


constant_component::constant_component(double value) : component(0, 0, 1), value(value) {}

output_value& constant_component::generate_output(int output) {
  output_value* output_value = new synthetisens::output_value;
  output_value->type = VALUE;
  output_value->value.parameter = this->value;
  return *output_value;
}

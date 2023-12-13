#define _USE_MATH_DEFINES

#include "component.h"
#include <cmath>
#include <iostream>

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

signal** component::get_input_signals(int sig_size) {
  signal** signals = new signal*[this->num_inputs];
  
  for (int i = 0; i < this->num_inputs; i++) {
    if (this->inputs[i].component == nullptr) signals[i] = new signal();
    else signals[i] = this->inputs[i].component->generate_output(this->inputs[i].port_sender, sig_size).value.signal;
  }

  return signals;
}

double* component::get_parameters() {
  double* parameters = new double[this->num_parameters];

  for (int i = 0; i < this->num_parameters; i++) {
    if (this->inputs[i + this->num_inputs].component == nullptr) parameters[i] = 0;
    else parameters[i] = this->inputs[i + this->num_inputs].component->generate_output(this->inputs[i + this->num_inputs].port_sender, 0).value.parameter;
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

output_value& speaker_component::generate_output(int output, int sig_size) {
  signal** inputs = get_input_signals(sig_size);
  signal* output_signal = &normalize(*inputs[0]);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = output_signal;
  return *output_value;
}

sinusoidal_component::sinusoidal_component() : component(0, 3, 1) {}

output_value& sinusoidal_component::generate_output(int output, int sig_size) {
  double* parameters = this->get_parameters();
  double* values = new double[sig_size];
  for (int i = 0; i < sig_size; i++) {
    values[i] = parameters[1] * sin(2 * M_PI * parameters[0] * i / SAMPLE_FREQ + parameters[2]);
  }
  signal* output_signal = new signal(sig_size, values);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = output_signal;
  return *output_value;
}

square_component::square_component() : component(0, 4, 1) {}

output_value& square_component::generate_output(int output, int sig_size) {
  double* parameters = this->get_parameters();
  double* values = new double[sig_size];
  if (1 - parameters[4] / PI_2 * sig_size > (double)sig_size / 2) {
    for (int i=0; i< 1 - parameters[4] / PI_2 * sig_size; i++ ){
      values[i] = parameters[1]; //max
    }
    for (int i = 1- parameters[4] / PI_2 * sig_size; i < ((double)sig_size / 2 + 1 - parameters[4] / PI_2 * sig_size); i++) {
        values[i] = parameters[2]; // min
    }
    for (int i = sig_size/2; i < sig_size; i++) {
        values[i] = parameters[1]; // max
    }
  } else {
    for (int i=0; i < (double)sig_size / 2 - parameters[4] / PI_2 * sig_size; i++ ){
      values[i] = parameters[2]; //min
    }
    for (int i = (double)sig_size / 2 - parameters[4] / PI_2 * sig_size; i < ((double)sig_size / 2 - parameters[4] / PI_2 * sig_size); i++) {
        values[i] = parameters[1]; // max
    }
    for (int i = (double)sig_size / 2 - parameters[4] / PI_2 * sig_size; i < sig_size; i++) {
        values[i] = parameters[1]; // min
    }
  }
  signal* output_signal = new signal(sig_size, values);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = output_signal;
  return *output_value;
}

triangle_component::triangle_component() : component(0, 3, 1) {}

output_value& triangle_component::generate_output(int output, int sig_size) {
  double* parameters = this->get_parameters();
  double* values = new double[sig_size];
   if (1 - parameters[4] / PI_2 * sig_size > (double)sig_size / 2){
    for (int i=0; i< 1 - parameters[4] / PI_2 * sig_size; i++ ){
      values[i] = ((-2 * parameters[1]) / sig_size) * i + (2 * parameters[2]) * (1 - parameters[2]/ PI_2); //descendante
    }
    for (int i = 1 - parameters[4] / PI_2 * sig_size; i < ((double)sig_size / 2 + 1 - parameters[4] / PI_2*sig_size); i++) {
        values[i] = ((-2 * parameters[1]) / sig_size) * i + (2 * parameters[1]) * (parameters[2] / PI_2 - 1) ; // montante
    }
    for (int i = sig_size/2; i < sig_size; i++) {
        values[i] = (-2 * parameters[1]) / sig_size * i + (2 * parameters[1]) * (0.5 - parameters[2] / PI_2) + parameters[1]; // descendante
    }
  } else {
    for (int i=0; i < (double)sig_size / 2 - parameters[4] / PI_2 * sig_size; i++ ){
      values[i] = ((2 * parameters[1]) / sig_size) * (parameters[2] / PI_2 * sig_size + i); //montante
    }
    for (int i = (double)sig_size / 2 - parameters[4] / PI_2 * sig_size; i < ((double)sig_size / 2 - parameters[4] / PI_2 * sig_size); i++) {
        values[i] = (-2 * parameters[1] / sig_size) * i + (2 * parameters[1]) * (1 - parameters[2] / PI_2); // descendante
    }
    for (int i = (double)sig_size / 2 - parameters[4] / PI_2 * sig_size; i < sig_size; i++) {
        values[i] = (2 * parameters[1] / sig_size) * i + (-2 * parameters[1]) * (1 - parameters[2] / PI_2); // montante
    }
  }
  signal* output_signal = new signal(sig_size, values);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = output_signal;
  return *output_value;
}

sawtooth_component::sawtooth_component() : component(0, 2, 1) {}

output_value& sawtooth_component::generate_output(int output, int sig_size) {
  double* parameters = get_parameters();
  double* values = new double[sig_size];
  for (int i = 0; i < sig_size; i++) {
      values[i] = (parameters[1]*i)/sig_size;
  }
  signal* output_signal = new signal(sig_size, values);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = output_signal;
  return *output_value;
}

sum_component::sum_component() : component(2, 0, 1) {}

output_value& sum_component::generate_output(int output, int sig_size) {
  signal** inputs = get_input_signals(sig_size);
  signal& output_signal = *inputs[0] + *inputs[1];
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = &output_signal;
  return *output_value;
}

sub_component::sub_component() : component(2, 0, 1) {}

output_value& sub_component::generate_output(int output, int sig_size) {
  signal** inputs = get_input_signals(sig_size);
  signal& output_signal = *inputs[0] - *inputs[1];
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = &output_signal;
  return *output_value;
}

prod_component::prod_component() : component(2, 0, 1) {}

output_value& prod_component::generate_output(int output, int sig_size) {
  signal** inputs = get_input_signals(sig_size);
  signal& output_signal = *inputs[0] * *inputs[1];
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = &output_signal;
  return *output_value;
}
div_component::div_component() : component(2, 0, 1) {}

output_value& div_component::generate_output(int output, int sig_size) {
  signal** inputs = get_input_signals(sig_size);
  signal& output_signal = *inputs[0] / *inputs[1];
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = &output_signal;
  return *output_value;
}

derivative_component::derivative_component() : component(1, 0, 1) {}

output_value& derivative_component::generate_output(int output, int sig_size) {
  signal** inputs = get_input_signals(sig_size);
  signal& output_signal = derivative(*inputs[0]);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = &output_signal;
  return *output_value;
}

normalize_component::normalize_component() : component(1, 0, 1) {}

output_value& normalize_component::generate_output(int output, int sig_size) {
  signal** inputs = get_input_signals(sig_size);
  signal& output_signal = normalize(*inputs[0]);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = &output_signal;
  return *output_value;
}

primitive_component::primitive_component() : component(1, 0, 1) {}

output_value& primitive_component::generate_output(int output, int sig_size) {
  signal** inputs = get_input_signals(sig_size);
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

output_value& custom_component::generate_output(int output, int sig_size) {
  double* nvalues = this->sig->get_values(0, sig_size);
  signal* output_signal = new signal(sig_size, nvalues);

  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = output_signal;
  return *output_value;
}


constant_component::constant_component(double value) : component(0, 0, 1), value(value) {}

output_value& constant_component::generate_output(int output, int sig_size) {
  output_value* output_value = new synthetisens::output_value;
  output_value->type = VALUE;
  output_value->value.parameter = this->value;
  return *output_value;
}

filter_component::filter_component() : component(1, 4, 1) {}

output_value& filter_component::generate_output(int output, int sig_size) {
  double* parameters = this->get_parameters(); 
  signal** inputs = get_input_signals(sig_size);
  double cutoff_freq = parameters[0] ;
  double z = parameters[1] ;
  int order = 1+ int(parameters[2])%8 ;
  int type = int(parameters[3]) ;
  double* filtered_values = new double[sig_size];
  for ( int i = 0 ; i < sig_size; i++ ){
    filtered_values[i] = inputs[0]->get_value(i);
  }
  double omega = PI_2*cutoff_freq ;
  double a = exp(-omega*z/SAMPLE_FREQ)*sin(omega/SAMPLE_FREQ);
  double b = -2*exp(-omega*z/SAMPLE_FREQ)*cos(omega/SAMPLE_FREQ);
  double c = exp(-2*z*omega/SAMPLE_FREQ);
  double value = 0 ;

  for ( int i = sig_size-1 ; i>0 ; i-- ){
    switch ( order )
      {
         case 1:
          value  = omega/SAMPLE_FREQ*inputs[0]->get_value(i) +
          (1 - omega/SAMPLE_FREQ)*filtered_values[i-1];
          break;

        case 2 :
          value = a*inputs[0]->get_value(i-1) - b*filtered_values[i-1]
          + c*filtered_values[i-2];
          break ;

        case 3 :
          value = a*inputs[0]->get_value(i-1) - b*filtered_values[i-1]
          + c*filtered_values[i-2];
          break ;

        case 4:
          value = pow(a, 2)*inputs[0]->get_value(i-1) 
          -2*b*filtered_values[i-1]
          - (pow(b,2) + 2*c)*filtered_values[i-2]
          - 2*b*c*filtered_values[i-3]
          - pow(c,2)*filtered_values[i-4];
          break;

        case 5 :
          value = pow(a, 2)*inputs[0]->get_value(i-1) 
          -2*b*filtered_values[i-1]
          - (pow(b,2) + 2*c)*filtered_values[i-2]
          - 2*b*c*filtered_values[i-3]
          - pow(c,2)*filtered_values[i-4];
          break;

        case 6 :
          value =  pow(a, 6)*inputs[0]->get_value(i-1)
          - 3*b*filtered_values[i-1]
          - 3*(b*b + c)*filtered_values[i-2]
          - (pow(b,3) + 6*b*c)*filtered_values[i-3]
          - 3*c*(b*b + c)*filtered_values[i-4]
          - 3*b*c*c*filtered_values[i-5]
          - pow(c,3)*filtered_values[i-6];
          break;

        case 7 :
          value =  pow(a, 6)*inputs[0]->get_value(i-1)
          - 3*b*filtered_values[i-1]
          - 3*(b*b + c)*filtered_values[i-2]
          - (pow(b,3) + 6*b*c)*filtered_values[i-3]
          - 3*c*(b*b + c)*filtered_values[i-4]
          - 3*b*c*c*filtered_values[i-5]
          - pow(c,3)*filtered_values[i-6];
          break ;

        case  8 :
          value = pow(a, 8)*inputs[0]->get_value(i-8)
          - 4*b*filtered_values[i-1]
          - (6*b*b + 4*c)*filtered_values[i-2]
          - (12*b*c + 4*pow(b,3))*filtered_values[i-3]
          - (pow(b,4) + 12*b*b*c + 6*c*c)*filtered_values[i-4]
          - (12*b*c*c + 4*pow(b,3)*c + 4*b*pow(c,3))*filtered_values[i-5]
          - (4*pow(c,3) + 6*b*b*c*c)*filtered_values[i-6]
          - 4*b*pow(c,3)*filtered_values[i-7]
          - pow(c, 4)*filtered_values[i-8];
          break ;
      }
      filtered_values[i] = value ;
      }
  signal* filtered_signal = new synthetisens::signal(sig_size, filtered_values);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = filtered_signal;
  return *output_value;
}

dist_component::dist_component() : component(1,2, 1) {}

output_value& dist_component::generate_output(int output, int sig_size) {
  double* parameters = this->get_parameters(); 
  signal** inputs = get_input_signals(sig_size);
  double gain = parameters[0] ;
  int type = int(parameters[1]) % 2 ;
  double * out_table = new double[sig_size];
  for( int i = 0 ; i < sig_size ; i++ ){
    switch (type)
    {
      case 0: // Soft saturation
        out_table[i] = 4/PI_2*atan((1 + gain)*inputs[0]->get_value(i));
        break;

      case 1 : // Hardclipping
        out_table[i] = (inputs[0]->get_value(0) > gain)? gain : inputs[0]->get_value(i);
        break;

      case 2 : // sinefold
        out_table[i] = sin(PI_2/4*(1 + gain)*inputs[0]->get_value(i)) ;
        break;
    }
  }
  signal* output_signal = new synthetisens::signal(sig_size, out_table);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = output_signal;
  return *output_value;
}

delay_component::delay_component() : component(1, 3, 1) {}

output_value& delay_component::generate_output(int output, int sig_size) {
  double* parameters = this->get_parameters(); 
  signal** inputs = get_input_signals(sig_size);
  int time = int(parameters[0]) ;
  double feedback = parameters[1] ;
  double mix = parameters[2] ;
  double * delay_tab = new double[sig_size];

  for ( int i = 0 ; i < sig_size; i++ ){
    delay_tab[i] = inputs[0]->get_value(i);
  }
  
  double alpha = log(10)/((feedback -1)*time);

  for(int j = 1 ; j < feedback ; j++){
    for(int i = 0 ; i < sig_size; i++){
      delay_tab[i] += mix*exp(-alpha*(j-1)*inputs[0]->get_value(i+j*time));
    }
  }
  signal* delay_signal = new synthetisens::signal(sig_size, delay_tab);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = delay_signal;
  return *output_value; 
}

keyboard_component::keyboard_component() : component(1, 1, 1) {
  this->pressed_keys = new bool[KEY_COUNT];

  for (int i = 0; i < KEY_COUNT; i++) {
    this->pressed_keys[i] = false;
  }
}

keyboard_component::~keyboard_component() {
  delete[] this->pressed_keys;
}

void keyboard_component::press_key(key k) {
  this->pressed_keys[k] = true;
}

void keyboard_component::release_key(key k) {
  this->pressed_keys[k] = false;
}

output_value& keyboard_component::generate_output(int output, int sig_size) {
  double* parameters = this->get_parameters();
  double freq = parameters[0];

  double* values = new double[sig_size];
  for (int i = 0; i < sig_size; i++) {
    values[i] = 0;
  }
  signal* output_signal = new signal(sig_size, values);

  for (int i = 0; i < KEY_COUNT; i++) {
    if (this->pressed_keys[i]) {
      double n_freq = note_freq[i];
      int req_size = sig_size * n_freq / freq; 

      signal** inputs = get_input_signals(req_size);

      output_signal = &(*output_signal + change_samplerate(*inputs[0], n_freq, freq));
    }
  }

  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = output_signal;
  return *output_value;
}

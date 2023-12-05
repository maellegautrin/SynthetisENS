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
    values[i] = parameters[1] * sin(PI_2 * parameters[0] * i / SAMPLE_FREQ + parameters[2]);
  }
  signal* output_signal = new signal(size, values, true);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = output_signal;
  return *output_value;
}

square_component::square_component() : component(0, 3, 1) {}

output_value& square_component::generate_output(int output) {
  double* parameters = this->get_parameters();
  int size = SAMPLE_FREQ / parameters[0];
  double* values = new double[size];
  for (int i = 0; i < (size/2); i++) {
      values[i] = parameters[2]; // min
  }
  for (int i = size/2; i < size; i++) {
      values[i] = parameters[1]; // max
  }
  signal* output_signal = new signal(size, values, true);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = output_signal;
  return *output_value;
}

triangle_component::triangle_component() : component(0, 2, 1) {}

output_value& triangle_component::generate_output(int output) {
  double* parameters = this->get_parameters();
  int size = SAMPLE_FREQ / parameters[0];
  double* values = new double[size];
  for (int i = 0; i < (size/2); i++) {
      values[i] = (parameters[1]*i*2)/size;
  }
  for (int i = size/2; i < size; i++) {
      values[i] = parameters[1]*(1-2*i/size);
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

constant_component::constant_component(double value) : component(0, 0, 1), value(value) {}

output_value& constant_component::generate_output(int output) {
  output_value* output_value = new synthetisens::output_value;
  output_value->type = VALUE;
  output_value->value.parameter = this->value;
  return *output_value;
}

filter_component::filter_component() : component(1,4, 1) {}

output_value& filter_component::generate_output(int output) {
  double* parameters = this->get_parameters(); 
  signal** inputs = get_input_signals();
  double cutoff_freq = parameters[0] ;
  double z = parameters[1] ;
  int order = 1+ int(parameters[2])%8 ;
  int type = int(parameters[3]) ;
  int size_table = inputs[0]->size ;
  double* filtered_values = new double[size_table];
  for ( int i = 0 ; i < size_table; i++ ){
    filtered_values[i] = inputs[0]->get_value(i);
  }
  double omega = PI_2*cutoff_freq ;
  double a = exp(-omega*z/SAMPLE_FREQ)*sin(omega/SAMPLE_FREQ);
  double b = -2*exp(-omega*z/SAMPLE_FREQ)*cos(omega/SAMPLE_FREQ);
  double c = exp(-2*z*omega/SAMPLE_FREQ);
  double value = 0 ;

  for ( int i = size_table-1 ; i>0 ; i-- ){
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
      filtered_signal[i] = value ;
      }
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = filtered_signal;
  return *output_value;
}

dist_component::dist_component() : component(1,2, 1) {}

output_value& dist_component::generate_output(int output) {
  double* parameters = this->get_parameters(); 
  signal** inputs = get_input_signals();
  double gain = parameters[0] ;
  int type = int(parameters[1]) % 2 ;
  double * out_table = new double[inputs[0]->size];
  for( int i = 0 ; i < inputs[0]->size ; i++ ){
    switch (type)
    {
      case 0: // Soft saturation
        out_table[i] = 4/PI_2*((1 + gain)*inputs[0]->get_value(i));
        break;

      case 1 : // Hardclipping
        out_table[i] = (inputs[0]->get_value(0) > gain)? gain : inputs[0]->get_value(i);
        break;

      case 2 : // sinefold
        out_table[i] = sin(PI_2/4*(1 + gain)*inputs[0]->get_value(i)) ;
        break;
    }
  }
  signal* output_signal = new synthetisens::signal(inputs[0]->size, out_table, inputs[0]->loop);
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = output_signal;
  return *output_value;
}

delay_component::delay_component() : component(1,3, 1) {}

output_value& delay_component::generate_output(int output) {
  double* parameters = this->get_parameters(); 
  signal** inputs = get_input_signals();
  int time = int(parameters[0]) ;
  double feedback = parameters[1] ;
  double mix = parameters[2] ;
  int size = inputs[0]->size ;
  double * delay_tab = new double[size];
  for ( int i = 0 ; i < size; i++ ){
    delay_tab[i] = inputs[0]->get_value(i);
  }
  
  signal* delay_signal = new synthetisens::signal(inputs[0]->size, delay_tab, false);
  double alpha = log(10)/((feedback -1)*time);

  for(int j = 1 ; j < feedback ; j++){
    double x = inputs[0]->tick(j*time);
    delay_signal = delay_signal + mix*exp(-alpha*(j-1)*inputs[0]);
  }
  output_value* output_value = new synthetisens::output_value;
  output_value->type = SIGNAL;
  output_value->value.signal = delay_signal;
  return *output_value; 
}




/*
 * Here is defined the component class
 * A component is a block with in/out signals and modulate it thanks to some parameters
 *
 * It can be a signal generator (#in = 0)
 * It can be a summer (#in = 2, #out = 1)
 * It can be a filter (#in = 1, #out = 1)
 *
 */

#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "signal.h"
#include <vector>

namespace synthetisens {

  typedef class component component_t; // typedef for input_port

  enum port_type {
    SIGNAL,
    VALUE
  };

  struct input_port {
    component_t* component;
    int port_sender;
  };

  struct output_value {
    port_type type;
    union {
      synthetisens::signal* signal;
      double parameter;
    } value;
  };

  class component {
    protected:
      input_port* inputs;

      signal** get_input_signals();
      double* get_parameters();

    public:
      const int num_inputs;
      const int num_parameters;
      const int num_outputs;

      component(int num_inputs, int num_parameters, int num_outputs);
      ~component();

      void connect_input(int input_num, component& input, int port_sender);
      void disconnect_input(int input_num);
      port_type get_port_type(int port_num) const;

      virtual output_value& generate_output(int port) = 0;
  };

  class sinusoidal_component : public component {
    public:
      sinusoidal_component();
      output_value& generate_output(int output) override;
  };

  class sum_component : public component {
    public:
      sum_component();
      output_value& generate_output(int output) override;
  };

  class constant_component : public component {
    public:
      const double value;

      constant_component(double value);
      output_value& generate_output(int output) override;
  };

}

#endif // _COMPONENT_H

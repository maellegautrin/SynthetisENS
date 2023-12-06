/*
 * Here is defined the Component class
 * A Component is a block with in/out signals and modulate it thanks to some parameters
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

  class signal_component : public component {
    private:
      synthetisens::signal& sig;
    public:
      signal_component(synthetisens::signal& sig);
      output_value& generate_output(int output) override;
  };

  class speaker_component : public component {
    public:
      speaker_component();
      output_value& generate_output(int output) override;
  };

  class sinusoidal_component : public component {
    public:
      sinusoidal_component();
      output_value& generate_output(int output) override;
  };


  class square_component : public component {
    public:
      square_component();
      output_value& generate_output(int output) override;
  };

  class triangle_component : public component {
    public:
      triangle_component();
      output_value& generate_output(int output) override;
  };

  class sawtooth_component : public component {
    public:
      sawtooth_component();
      output_value& generate_output(int output) override;
  };

  class sum_component : public component {
    public:
      sum_component();
      output_value& generate_output(int output) override;
  };

  class sub_component : public component {
    public:
      sub_component();
      output_value& generate_output(int output) override;
  };

  class prod_component : public component {
    public:
      prod_component();
      output_value& generate_output(int output) override;
  };

  class div_component : public component {
    public:
      div_component();
      output_value& generate_output(int output) override;
  };

  class derivative_component : public component {
    public:
      derivative_component();
      output_value& generate_output(int output) override;
  };

  class normalize_component : public component {
    public:
      normalize_component();
      output_value& generate_output(int output) override;
  };

  class primitive_component : public component {
    public:
      primitive_component();
      output_value& generate_output(int output) override;
  };

  class constant_component : public component {
    public:
      double value;

      constant_component(double value);
      output_value& generate_output(int output) override;
  };

  class filter_component : public component {
    public:
      filter_component();
      output_value& generate_output(int output) override;
  };

  class dist_component : public component {
    public:
      dist_component();
      output_value& generate_output(int output) override;
  };

  class delay_component : public component {
    public:
      delay_component();
      output_value& generate_output(int output) override;
  };


}

#endif // _Component_H

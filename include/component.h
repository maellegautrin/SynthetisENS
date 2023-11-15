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

  class component {
    protected:
      component** inputs;
      std::vector<component*>* outputs;

      signal** get_input_signals();
      signal& get_output_signal(component* output);

    public:
      const int num_inputs;
      const int num_outputs;

      component(int num_inputs, int num_outputs);
      ~component();

      virtual signal& generate_signal(int output) = 0;
  };

  class sinusoidal_component : public component {
    public:
      double frequency;

      sinusoidal_component(double frequency);
      signal& generate_signal(int output) override;
  };

  class sum_component : public component {
    public:
      sum_component(component& c1, component& c2);
      signal& generate_signal(int output) override;
  };

}

#endif // _COMPONENT_H

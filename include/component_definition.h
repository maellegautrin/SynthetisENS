#ifndef _COMPONENT_DEFINITION_H_
#define _COMPONENT_DEFINITION_H_

#include "component.h"

namespace synthetisens {

  enum ComponentValue {
    SINUSOIDAL_GENERATOR,
    SQUARE_GENERATOR,
    TRIANGLE_GENERATOR,
    SAWTOOTH_GENERATOR,

    SUM,
    SUB,
    MUL,
    DIV,
    DERIVATIVE,
    PRIMITIVE,

    CONSTANT,
    SPEAKER,
    CUSTOM,

    COMPONENT_VALUE_COUNT
  };

  enum ComponentType {
    GENERATOR,
    FILTER,
    OPERATOR,
    OTHER
  };

  extern std::string component_icon[];
  extern ComponentType component_type[];

  component* create_component(ComponentValue value);

}

#endif // !_COMPONENT_DEFINITION_H_

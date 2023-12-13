#ifndef _COMPONENT_DEFINITION_H_
#define _COMPONENT_DEFINITION_H_

#include "component.h"
#include <string>

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
    
    FILTERC,
    
    CONSTANT,
    SPEAKER,
    KEYBOARD,
    CUSTOM,

    COMPONENT_VALUE_COUNT
  };

  enum ComponentType {
    GENERATOR,
    FILTER,
    OPERATOR,
    OTHER,

    COMPONENT_TYPE_COUNT
  };

  extern std::string component_icon[];
  extern ComponentType component_type[];

  component* create_component(ComponentValue value);


  extern std::string component_type_name[];

}

#endif // !_COMPONENT_DEFINITION_H_

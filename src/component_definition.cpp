#include "component_definition.h"
#include "component.h"

using namespace synthetisens;

extern component* speaker;

std::string synthetisens::component_icon[] = {
  "img/sine.png",
  "img/square.png",
  "img/triangle.png",
  "img/sawtooth.png",

  "img/sum.png",
  "img/sub.png",
  "img/product.png",
  "img/div.png",
  "img/derivative.png",
  "img/primitive.png",

  "img/const440.png",
  "img/speaker.png",
  "img/input.png"
};

ComponentType synthetisens::component_type[] = {
  GENERATOR,
  GENERATOR,
  GENERATOR,
  GENERATOR,

  OPERATOR,
  OPERATOR,
  OPERATOR,
  OPERATOR,
  OPERATOR,
  OPERATOR,

  OTHER,
  OTHER,
  OTHER
};

component* synthetisens::create_component(ComponentValue value) {
  switch (value) {
    case SINUSOIDAL_GENERATOR:
      return new sinusoidal_component();
    case SQUARE_GENERATOR:
      return new square_component();
    case TRIANGLE_GENERATOR:
      return new triangle_component();
    case SAWTOOTH_GENERATOR:
      return new sawtooth_component();
    case SUM:
      return new sum_component();
    case SUB:
      return new sub_component();
    case MUL:
      return new prod_component();
    case DIV:
      return new div_component();
    case DERIVATIVE:
      return new derivative_component();
    case PRIMITIVE:
      return new primitive_component();
    case CONSTANT:
      return new constant_component(0);
    case SPEAKER:
      speaker = new speaker_component();
      return speaker;
    case CUSTOM:
      return new custom_component();
    default:
      return nullptr;
  }
}

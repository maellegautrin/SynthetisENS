#ifndef _COMPONENT_SELECTOR_H
#define _COMPONENT_SELECTOR_H

#include "component.h"
#include "gtkmm/box.h"
#include "gtkmm/button.h"
#include "gtkmm/image.h"


namespace synthetisens {

  typedef enum {
    SIGNAL_COMPONENT,
    FILTER_COMPONENT,
    OPERATOR_COMPONENT,
    OTHER_COMPONENT
  } ComponentType;

  class ComponentSelector : public Gtk::Button {
    private:
      Gtk::Image* img;

    public:
      ComponentSelector(char* imglink, ComponentType type, int component_id);
      void click_handler();

      const int component_id;
      const ComponentType type;
      char* imglink;
  };
}
#endif

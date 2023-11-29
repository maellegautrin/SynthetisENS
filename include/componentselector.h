#ifndef _COMPONENT_SELECTOR_H
#define _COMPONENT_SELECTOR_H

#include "component.h"
#include "gtkmm/box.h"
#include "gtkmm/button.h"
#include "gtkmm/frame.h"
#include "gtkmm/image.h"
#include "gtkmm/eventbox.h"
#include "component_definition.h"


namespace synthetisens {

  class ComponentSelector : public Gtk::EventBox {
    private:
      Gtk::Image* img;
      void component_data_drag(const Glib::RefPtr<Gdk::DragContext>& context, Gtk::SelectionData& selection_data, guint info, guint time);

    public:
      ComponentSelector(ComponentValue value);

      const ComponentValue value;
  };
}
#endif

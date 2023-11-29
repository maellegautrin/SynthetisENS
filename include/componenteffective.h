#ifndef _COMPONENT_EFFECTIVE_H
#define _COMPONENT_EFFECTIVE_H

#include "component.h"
#include "componentselector.h"
#include "gtkmm/eventbox.h"
#include "gtkmm/grid.h"
#include "gtkmm/frame.h"
#include "gtkmm/button.h"
#include "gtkmm/widget.h"
#include "gtkmm/image.h"
#include "gtkmm/box.h"
#include <list>
#include <vector>


namespace synthetisens{

  enum PortType {
    SIGNAL_INPUT_PORT,
    SIGNAL_OUTPUT_PORT,
    VALUE_INPUT_PORT,
    VALUE_OUTPUT_PORT
  };

  typedef class Port Port;
  typedef class ComponentEffective ComponentEffective;

  struct Point {
    int x;
    int y;
  };

  struct Line {
    Point start;
    Point end;
  };

  struct Wire {
    Port* source;
    Port* destination;
  };

  class Port : public Gtk::EventBox {
    private:
      ComponentEffective* parent;
      std::vector<Wire*> wires;
      Gtk::Frame* frame;

      void drag_data_get(const Glib::RefPtr<Gdk::DragContext>& context, Gtk::SelectionData& selection_data, guint info, guint time);
      void drag_data_recvd(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);

    public:
      Port(PortType type, ComponentEffective* parent);

      const PortType type;
      void link(Port* destination);
      void outlink(Port* destination);
      void inlink(Port* destination);

      std::vector<Line> draw_wires();
  };

  class ComponentEffective : public Gtk::Grid {
    private:
      Gtk::Box* box;
      Gtk::Image* img;
      Port** input_ports;
      Port** output_ports;

    public:
      ComponentEffective(ComponentValue value);

      const ComponentValue value;
      component* virtual_component;

      int port_position(Port* port);
      std::vector<Line> draw_ports();
  };
}
#endif

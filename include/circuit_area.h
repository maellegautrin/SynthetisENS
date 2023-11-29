#ifndef _CIRCUIT_AREA_H
#define _CIRCUIT_AREA_H

#include "gtkmm/layout.h"
#include "componenteffective.h"
#include <vector>

using namespace std;

namespace synthetisens {

  class CircuitArea : public Gtk::Layout {
    private:
      bool wire_draw(const Cairo::RefPtr<Cairo::Context>& cr);
      void drag_data_recvd(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);

    public:
      CircuitArea();
      ~CircuitArea();
  };

}

#endif // !_CIRCUIT_AREA_H

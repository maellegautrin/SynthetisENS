#include "circuit_area.h"
#include "componenteffective.h"
#include "gtkmm/targetentry.h"
#include <vector>
#include "component_definition.h"

using namespace synthetisens;
using namespace std;

CircuitArea::CircuitArea() {
  this->set_size(900, 600);

  // Drag and drop
  this->drag_dest_set(vector<Gtk::TargetEntry>({Gtk::TargetEntry("placing_component")}), Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_COPY);
  this->signal_drag_data_received().connect(sigc::mem_fun(*this, &CircuitArea::drag_data_recvd));

  this->signal_draw().connect(sigc::mem_fun(*this, &CircuitArea::wire_draw));
}

CircuitArea::~CircuitArea() {}

bool CircuitArea::wire_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
  Gtk::Allocation allocation = this->get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  for (int i = 0; i < this->get_children().size(); i++) {
    ComponentEffective* comp = (ComponentEffective*) this->get_children()[i];
    vector<Line> lines = comp->draw_ports();
    for (int j = 0; j < lines.size(); j++) {
      cr->move_to(lines[j].start.x, lines[j].start.y);
      cr->line_to(lines[j].end.x, lines[j].end.y);
      cr->stroke();
    }
  }
  return true;
}

void CircuitArea::drag_data_recvd(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time) {
  ComponentValue value = static_cast<ComponentValue>(std::atoi((const char*) selection_data.get_data()));

  ComponentEffective* comp = new ComponentEffective(value);

  this->put(*comp, x, y);
  this->show_all();

  context->drag_finish(true, false, time);
}

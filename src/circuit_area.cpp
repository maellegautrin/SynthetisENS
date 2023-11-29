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
  cout << "Hello World!" << endl;
  Gtk::Allocation allocation = this->get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  

  // cr->set_source_rgb(0.0, 0.0, 0.0);
  // cr->rectangle(10, 10, width/2, height/2);
  // cr->fill();

  return true;
}

void CircuitArea::drag_data_recvd(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time) {
  ComponentValue value = static_cast<ComponentValue>(std::atoi((const char*) selection_data.get_data()));

  ComponentEffective* comp = new ComponentEffective(value);

  this->put(*comp, x, y);
  this->show_all();

  context->drag_finish(true, false, time);
}

// void CircuitArea::add_wire(Port* source, Port* destination) {
//   Wire* wire = new Wire();
//   wire->source = source;
//   wire->destination = destination;
//   this->wires.push_back(wire);
// }

// void CircuitArea::remove_wires(Port* port) {
//   for (int i = 0; i < this->wires.size(); i++) {
//     if (this->wires[i]->source == port || this->wires[i]->destination == port) {
//       this->wires.erase(this->wires.begin() + i);
//       i--;
//     }
//   }
// }

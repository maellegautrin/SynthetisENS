#include "componentselector.h"
#include "gdkmm/types.h"
#include "gtkmm/enums.h"
#include "gtkmm/image.h"
#include <vector>
#include "component_definition.h"

using namespace synthetisens;

ComponentSelector::ComponentSelector(ComponentValue value) : value(value) {
  this->set_size_request(125,125);
  this->img = new Gtk::Image(component_icon[this->value]);
  this->img->set_valign(Gtk::ALIGN_CENTER);
  this->img->set_halign(Gtk::ALIGN_CENTER);
  this->add(*this->img);
  // this->set_image(*(this->img)); 

  this->drag_source_set(std::vector<Gtk::TargetEntry>({Gtk::TargetEntry("placing_component")}), Gdk::BUTTON1_MASK, Gdk::ACTION_COPY);
  this->signal_drag_data_get().connect(sigc::mem_fun(*this, &ComponentSelector::component_data_drag));
}

void ComponentSelector::component_data_drag(const Glib::RefPtr<Gdk::DragContext>& context, Gtk::SelectionData& selection_data, guint info, guint time){
  selection_data.set(selection_data.get_target(), std::to_string(this->value));
}

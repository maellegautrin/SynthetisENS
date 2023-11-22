#include "signal_viewer.h"

using namespace synthetisens;

SignalViewer::SignalViewer(const int width, const int height, signal* sig) : width(width), height(height), sig(sig) {
  this->set_size_request(width, height);
  this->signal_draw().connect(sigc::mem_fun(*this, &SignalViewer::on_draw));
}

bool SignalViewer::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
  Gtk::Allocation allocation = get_allocation();
  const double width = allocation.get_width();
  const double height = allocation.get_height();

  double stepX = width / (double)sig->size;
  double max = sig->get_max();
  double stepY = height / (2 * max);

  cr->set_line_width(1.0);
  cr->set_source_rgb(0.0, 0.0, 0.0);
  
  //placing the first signal point
  cr->move_to(0, height / 2 - sig->get_value(0) * stepY);

  //drawing the signal
  for (int i = 1; i < 2 * sig->size; i++) {
    cr->line_to(i * stepX, height / 2 - sig->get_value(i) * stepY);
  }
  cr->stroke();

  return true;
}

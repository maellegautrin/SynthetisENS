#ifndef _SIGNAL_VIEWER_H
#define _SIGNAL_VIEWER_H

#include "signal.h"
#include "gtkmm/drawingarea.h"

using namespace Gtk;

namespace synthetisens {

  class SignalViewer : public DrawingArea {
    private:
      const int width;
      const int height;
      signal* sig;

    public:
      SignalViewer(const int width, const int height, signal* sig);
      bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
  };

}

#endif

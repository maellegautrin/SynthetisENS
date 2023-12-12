#ifndef _ICONBAR_H
#define _ICONBAR_H


#include "gtkmm/toolbar.h"
namespace synthetisens {

  class IconBar : public Gtk::Toolbar {
    private:
      Gtk::ToolButton* play;
      Gtk::ToolButton* stop;
      Gtk::ToolButton* save;

    public:
      IconBar();
  };

}

#endif // !_ICONBAR_H

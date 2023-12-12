#ifndef _MENUBAR_H_
#define _MENUBAR_H_

#include "gtkmm/menubar.h"
#include "gtkmm/menu.h"
#include "gtkmm/menuitem.h"

namespace synthetisens {

  class MenuBar : public Gtk::MenuBar {
    private:
      Gtk::MenuItem* file;
      Gtk::Menu* file_menu;
      Gtk::MenuItem* play;
      Gtk::MenuItem* save;
      Gtk::MenuItem* duration;

      Gtk::MenuItem* edit;

      Gtk::MenuItem* view;
      Gtk::Menu* view_menu;
      Gtk::MenuItem* preview;

    public:
      MenuBar();
  };

}

#endif // !_MENUBAR_H_

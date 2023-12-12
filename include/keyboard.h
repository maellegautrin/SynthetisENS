#ifndef _KEYBOARD_H
#define _KEYBOARD_H



#include "gtkmm/frame.h"
#include "gtkmm/image.h"
#include "gtkmm/button.h"
#include "gtkmm/box.h"
#include "gtkmm/eventbox.h"
#include "gtkmm/layout.h"
#include "gdk/gdkkeysyms.h"
#include "note.h"


namespace synthetisens {

  class Key : public Gtk::Frame {
    public:
      Key(key note);
      void pressed();
      void released();

    private:
      Gtk::Image* image;
      key note;
  };

  class Keyboard : public Gtk::EventBox {
    public:
      Keyboard();
      bool press(GdkEventKey* key);
      bool release(GdkEventKey* key);

    private:
      Gtk::Layout* layout;
      Key** keylist;

};


}


#endif

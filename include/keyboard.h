#ifndef _KEYBOARD_H
#define _KEYBOARD_H



#include "gtkmm/frame.h"
#include "gtkmm/image.h"
#include "gtkmm/button.h"
#include "gtkmm/box.h"
#include "gtkmm/eventbox.h"
#include "gtkmm/layout.h"
#include "gdk/gdkkeysyms.h"


namespace synthetisens {


  enum note_t {
    C = 0,
    Cs,
    D,
    Ds,
    E,
    F,
    Fs,
    G,
    Gs,
    A,
    As,
    B
  };

  enum key_t{
    WHITE_KEY,
    BLACK_KEY
  };

  extern double note_freq[];

  class Key : public Gtk::Frame {
    public:
    Key(note_t note, key_t type);
    void pressed();
    void released();

    private:
    Gtk::Image* image;
    note_t note;
    key_t type;


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
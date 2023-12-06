#ifndef _KEYBOARD_H
#define _KEYBOARD_H



#include "gtkmm/action.h"
#include "gtkmm/button.h"
#include "gtkmm/box.h"
#include "gtkmm/eventbox.h"
#include "gtkmm/layout.h"
#include "gdk/gdkkeysyms.h"


namespace synthetisens {

  typedef enum {
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
  } note_t;

  typedef enum {
    WHITE_KEY,
    BLACK_KEY
  } KeyType;

  double freq[12] = {
    130.81,
    138.59,
    146.83,
    155.56,
    164.81,
    174.61,
    185.0,
    196.0,
    207.65,
    220.0,
    233.08,
    246.94
  };

  class Key : public Gtk::EventBox {
    public:
    Key(note_t note, KeyType type);

    void click_handler();

    private:
    Gtk::Image* image;
    note_t note;
    KeyType type;


  };

  class Keyboard : public Gtk::Layout {
    public:
    Keyboard();

    private:
    Key** keylist;

}


#endif
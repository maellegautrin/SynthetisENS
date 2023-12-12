#include "keyboard.h"
#include "gdkmm/pixbuf.h"
#include "gtkmm/image.h"
#include "componentselector.h"
#include "gtkmm/layout.h"
#include <iostream>
#include "gdk/gdkkeysyms.h"

using namespace synthetisens;

int match_key(int keyval);

double note_freq[] = {
    130.81, //C
    138.59, //C sharp
    146.83, //D
    155.56, //D sharp
    164.81, //E
    174.61, //E sharp
    185.0,  //F sharp
    196.0,  //G
    207.65, //A flat
    220.0,  //A
    233.08,//A sharp
    246.94 //C flat
  };


Key::Key(note_t note, key_t type) : note(note), type(type) {
  switch (type) {
    case WHITE_KEY:
    this->image = new Gtk::Image("img/whitekey.png");
    break;
    case BLACK_KEY:
    this->image = new Gtk::Image("img/blackkey.png");
  
  }
  this->add(*this->image);    

}

void Key::pressed(){
  switch (type) {
    case WHITE_KEY:
    this->image->set(Gdk::Pixbuf::create_from_file("img/whitekey_p.png"));
    break;
    case BLACK_KEY:
    this->image->set(Gdk::Pixbuf::create_from_file("img/blackkey_p.png"));
  
  }
  this->image->queue_draw();

}

void Key::released(){
  switch (type) {
    case WHITE_KEY:
    this->image->set(Gdk::Pixbuf::create_from_file("img/whitekey.png"));
    break;
    case BLACK_KEY:
    this->image->set(Gdk::Pixbuf::create_from_file("img/blackkey.png"));
  }
  this->image->queue_draw();
}


Keyboard::Keyboard(){





  keylist = new Key*;
  keylist[0] = new Key((note_t)0,WHITE_KEY);
  keylist[1] = new Key(Cs,BLACK_KEY);
  keylist[2] = new Key(D,WHITE_KEY);
  keylist[3] = new Key(Ds,BLACK_KEY);
  keylist[4] = new Key(E,WHITE_KEY);
  keylist[5] = new Key(F,WHITE_KEY);
  keylist[6] = new Key(Fs,BLACK_KEY);
  keylist[7] = new Key(G,WHITE_KEY);
  keylist[8] = new Key(Gs,BLACK_KEY);
  keylist[9] = new Key(A,WHITE_KEY);
  keylist[10] = new Key(As,BLACK_KEY);
  keylist[11] = new Key(B,WHITE_KEY);

  layout = new Gtk::Layout();

  layout->put(*keylist[0],0,0);
  
  layout->put(*keylist[2],80,0);
  
  layout->put(*keylist[4],160,0);
  layout->put(*keylist[5],240,0);
  
  layout->put(*keylist[7],320,0);
  
  layout->put(*keylist[9],400,0);
  
  layout->put(*keylist[11],480,0);

  layout->put(*keylist[1],55,0);
  layout->put(*keylist[3],135,0);
  layout->put(*keylist[6],295,0);
  layout->put(*keylist[8],375,0);
  layout->put(*keylist[10],455,0);

  add(*layout);

}


bool Keyboard::press(GdkEventKey* event){
  int k = match_key(event->keyval);
  if(k<0) return false;
  
  keylist[k]->pressed();
  return true;
  }
bool Keyboard::release(GdkEventKey* event){
  int k = match_key(event->keyval);
  if(k<0) return false;
  
  keylist[k]->released();
  return true;
}


int match_key(int keyval){
  switch (keyval) {
    case GDK_KEY_q:
      
      return 0;
      break;
    case GDK_KEY_z:
      return 1;
      break;
    case GDK_KEY_s:
      return 2;
      break;
    case GDK_KEY_e:
      return 3;
      break;
    case GDK_KEY_d:
      return 4;
      break;
    case GDK_KEY_f:
      return 5;
      break;
    case GDK_KEY_t:
      return 6;
      break;
    case GDK_KEY_g:
      return 7;
      break;
    case GDK_KEY_y:
      return 8;
      break;
    case GDK_KEY_h:
      return 9;
      break;
    case GDK_KEY_u:
      return 10;
      break;
    case GDK_KEY_j:
      return 11;
      break;
    default:
      return -1;
  
  }
}
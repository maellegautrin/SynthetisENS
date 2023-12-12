#include "keyboard.h"
#include "gdkmm/pixbuf.h"
#include "gtkmm/image.h"
#include "gtkmm/layout.h"
#include "gdk/gdkkeysyms.h"
#include "component.h"

using namespace synthetisens;

extern synthetisens::speaker_component* speaker;
extern synthetisens::keyboard_component* keyboard;

int match_key(int keyval);

Key::Key(key note) : note(note) {
  switch (key_types[note]) {
    case WHITE_KEY:
      this->image = new Gtk::Image("img/whitekey.png");
      break;
    case BLACK_KEY:
      this->image = new Gtk::Image("img/blackkey.png");
      break;
  }
  this->add(*this->image);    
}

void Key::pressed(){
  keyboard->press_key(note);
  speaker->update_signal = true;

  switch (key_types[note]) {
    case WHITE_KEY:
      this->image->set(Gdk::Pixbuf::create_from_file("img/whitekey_p.png"));
      break;
    case BLACK_KEY:
      this->image->set(Gdk::Pixbuf::create_from_file("img/blackkey_p.png"));
      break;
  }
  this->image->queue_draw();
}

void Key::released(){
  keyboard->release_key(note);
  speaker->update_signal = true;

  switch (key_types[note]) {
    case WHITE_KEY:
      this->image->set(Gdk::Pixbuf::create_from_file("img/whitekey.png"));
      break;
    case BLACK_KEY:
      this->image->set(Gdk::Pixbuf::create_from_file("img/blackkey.png"));
      break;
  }
  this->image->queue_draw();
}


Keyboard::Keyboard(){
  keylist = new Key*[KEY_COUNT];

  for(int i=0;i<KEY_COUNT;i++){
    keylist[i] = new Key((key)i);
  }

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

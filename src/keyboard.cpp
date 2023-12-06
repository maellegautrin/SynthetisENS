#include "keyboard.h"
#include "gtkmm/image.h"
#include "componentselector.h"
#include <iostream>

using namespace synthetisens;


void Key::click_handler(){
  std::cout << this->note;
}

Key::Key(note_t note, KeyType type) : note(note), type(type) {
  switch (type) {
    case WHITE_KEY:
    this->image = new Gtk::Image("whitekey.png");
    break;
    case BLACK_KEY:
    this->image = new Gtk::Image("blackkey.png");
  
  }
  this->signal_clicked().connect(sigc::mem_fun(*this, &ComponentSelector::click_handler));
    

}


Keyboard::Keyboard(){

  keylist = new Key*;
  keylist[0] = new Key(C,WHITE_KEY);
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

  put(*keylist[0],0,0);
  put(*keylist[1],40,0);
  put(*keylist[2],80,0);
  put(*keylist[3],120,0);
  put(*keylist[4],160,0);
  put(*keylist[5],240,0);
  put(*keylist[6],280,0);
  put(*keylist[7],320,0);
  put(*keylist[8],360,0);
  put(*keylist[9],400,0);
  put(*keylist[10],440,0);
  put(*keylist[11],480,0);

}
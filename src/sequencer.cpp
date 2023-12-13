#include "sequencer.h"
#include "sigc++/functors/mem_fun.h"
#include <set>

std::set<int> whites = {0,2,4,5,7,9,11};

using namespace synthetisens;

void SequencerCell::image_update(){
  if(line->values[id]) {
    if((id + line->id) % 2) this->image->set(Gdk::Pixbuf::create_from_file("img/cell_dark_on.png"));
    else this->image->set(Gdk::Pixbuf::create_from_file("img/cell_bright_on.png"));
  }
  else {
    if((id + line->id) % 2) this->image->set(Gdk::Pixbuf::create_from_file("img/cell_dark.png"));
    else this->image->set(Gdk::Pixbuf::create_from_file("img/cell_bright.png"));
  }
}

bool SequencerCell::hovered(GdkEventCrossing* event){
  switch (event->state) {
    case GDK_SHIFT_MASK:
      this->line->values[id] = !this->line->values[id];
      image_update();
      return true;
    default:
      return false;
  }
}

bool SequencerCell::clicked(GdkEventButton* event){
  if(event->button == 1){
    this->line->values[id] = !this->line->values[id];
    image_update();
    return true;
  }
  return false;
}


SequencerCell::SequencerCell(int id, SequencerLine* line) : id(id), line(line) {
  if((id + line->id) % 2) image = new Gtk::Image("img/cell_dark.png");
  else image = new Gtk::Image("img/cell_bright.png");
  add(*image);

  this->signal_enter_notify_event().connect(sigc::mem_fun(*this, &SequencerCell::hovered));
  this->signal_button_release_event().connect(sigc::mem_fun(*this,&SequencerCell::clicked));
}



SequencerLine::SequencerLine(int id, int length) : id(id), length(length) {
  values = new bool[length];
  cells = new SequencerCell*[length];
  for(int i = 0; i < length; i++){
    values[i] = false;
    cells[i] = new SequencerCell(i,this);
    this->attach(*cells[i],i,0,1,1);
  }
}

Sequencer::Sequencer(int range, int length) : range(range), length(length) {
  lines = new SequencerLine*[range];
  for(int i = 0; i < range; i++) {
    lines[i] = new SequencerLine(i,length);
    this->attach(*lines[i],1,i,1,1);
    if(whites.find(i % 12) != whites.end()) this->attach(*(new Gtk::Image("img/sequencer_keyboard_white.png")),0,i,1,1);
    else this->attach(*(new Gtk::Image("img/sequencer_keyboard_black.png")),0,i,1,1);
  }

}


#include "sequencer.h"

using namespace synthetisens;

void SequencerCell::image_update(){
  if(line->values[id]) this->image->set(Gdk::Pixbuf::create_from_file("img/cell_on.png"));
  else {
    if((id + line->id) % 2) this->image->set(Gdk::Pixbuf::create_from_file("img/cell_dark.png"));
    else this->image->set(Gdk::Pixbuf::create_from_file("img/cell_bright.png"));
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

  this->signal_button_release_event().connect(sigc::mem_fun(*this, &SequencerCell::clicked));
}



SequencerLine::SequencerLine(int id, int length) : id(id), length(length) {
  for(int i = 0; i < length; i++){
    values[i] = false;
    cells[i] = new SequencerCell(i,this);
  }
}

Sequencer::Sequencer(int range, int length) : range(range), length(length) {
  for(int i = 0; i < range; i++) lines[i] = new SequencerLine(i,length);

}
#ifndef _SEQUENCER_H
#define _SEQUENCER_H

#include "gtkmm/grid.h"
#include "gtkmm/image.h"
#include "gtkmm/eventbox.h"
//#include "keyboard.h"

#include "gtkmm/widget.h"



namespace synthetisens {


  typedef class SequencerCell SequencerCell;
  typedef class SequencerLine SequencerLine;


  class SequencerCell : public Gtk::EventBox {
    public:
      SequencerCell(int id, SequencerLine* line);
      void image_update();
      bool clicked(GdkEventButton* event);
      
    
    private:
      int id;
      SequencerLine* line;
      Gtk::Image* image;

  };

  class SequencerLine : public Gtk::Grid {

    public:
      SequencerLine(int id, int length);

      int id;
      bool* values;
      

    private:

      int length;
      SequencerCell* cells[15];
  };

  class Sequencer : public Gtk::Grid {
    public:
      Sequencer(int range, int length);

    private:
      int range;
      int length;
      SequencerLine* lines[12];

  };

}

#endif

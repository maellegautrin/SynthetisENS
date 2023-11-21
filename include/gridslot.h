#ifndef _GRID_SLOT_H
#define _GRID_SLOT_H


#include "component.h"
#include "gtkmm/button.h"
#include "gtkmm/frame.h"
#include "gtkmm/grid.h"



namespace synthetisens {


class GridSlot : public Gtk::Frame {
    public:
        GridSlot(Gtk::Grid* grid, int i, int j);
        void click_handler();

    private:

      Gtk::Button* slot_button;
    
};


};


#endif
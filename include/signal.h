/*
 * Here is defined the signal class
 *
 * A signal is the sound function transfered from a component to another
 *
 */
#ifndef _SIGNAL_H
#define _SIGNAL_H

#include "SineWave.h"

namespace synthetisens {

  class signal {
    public:
      signal operator+(signal other);

      virtual double tick();
  };

  class sum_signal : public signal {
    private:
      signal sig1;
      signal sig2;

    public:
      sum_signal(signal sig1, signal sig2);
      double tick();
  };

  class sin_signal : public signal {
    private:
      stk::SineWave sine;
      int time;

    public:
      sin_signal(double freq);
      double tick();
  };

}

#endif // _SIGNAL_H

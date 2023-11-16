/*
 * Here is defined the signal class
 *
 * A signal is the sound function transfered from a component to another
 *
 */
#ifndef _SIGNAL_H
#define _SIGNAL_H

#define SAMPLE_FREQ 44100
#include "SineWave.h"

namespace synthetisens {

  class signal {
    private:
      int position;
      const double* values;
      
    public:
      const int size;
      const bool loop;

      signal();
      signal(int size, double* values);
      signal(int size, double* values, bool loop);
      ~signal();
      
      double get_value(int position) const;

      double tick();
      void reset();
  };

}

synthetisens::signal& operator+(const synthetisens::signal& sig1, const synthetisens::signal& sig2);
synthetisens::signal& operator-(const synthetisens::signal& sig1, const synthetisens::signal& sig2);
synthetisens::signal& operator*(const synthetisens::signal& sig1, const synthetisens::signal& sig2);
synthetisens::signal& operator/(const synthetisens::signal& sig1, const synthetisens::signal& sig2);

#endif // _SIGNAL_H

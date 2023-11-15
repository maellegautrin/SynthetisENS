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
      int size;
      int position;
      double* values;
      bool loop;
      
    public:
      signal();
      signal(int size, double* values);
      signal(int size, double* values, bool loop);
      ~signal();
      
      int get_size() const;
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

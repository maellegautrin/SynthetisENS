/*
 * Here is defined the signal class
 *
 * A signal is the sound function transfered from a component to another
 *
 */
#ifndef _SIGNAL_H
#define _SIGNAL_H

#define SAMPLE_FREQ 44100

namespace synthetisens {

  class signal {
    private:
      int position;
      const double* values;
      
    public:
      const int size;

      signal();
      signal(int size, double* values);
      ~signal();
      
      double get_value(int position) const;
      double* get_values() const;
      double get_max() const;

      double tick();
      void reset();
  };

}


synthetisens::signal& operator+(const synthetisens::signal& sig1, const synthetisens::signal& sig2);
synthetisens::signal& operator-(const synthetisens::signal& sig1, const synthetisens::signal& sig2);
synthetisens::signal& operator*(const synthetisens::signal& sig1, const synthetisens::signal& sig2);
synthetisens::signal& operator/(const synthetisens::signal& sig1, const synthetisens::signal& sig2);
synthetisens::signal& derivative(const synthetisens::signal& sig);
synthetisens::signal& normalize(const synthetisens::signal& sig);
synthetisens::signal& primitive(const synthetisens::signal& sig);
synthetisens::signal& change_samplerate(const synthetisens::signal& sig, int old_samplerate, int new_samplerate);
#endif // _SIGNAL_H

#include "signal.h"
#include "SineWave.h"
#include <algorithm>
#include <iostream>
#include <math.h>

#define PI 3.14159265358979323846

using namespace std;
using namespace synthetisens;

signal::signal() : signal(0, new double[0]) {}

signal::signal(int size, double* values) : signal(size, values, false) {}

signal::signal(int size, double* values, bool loop) : size(size), position(0), values(values), loop(loop) {}

signal::~signal() {
  delete[] values;
}

double signal::get_value(int position) const {
  if (position < 0 || position >= this->size) {
    if (!this->loop) return 0;
    int pos = position % this->size;
    if (pos < 0) pos += this->size;
    return this->values[pos];
  }
  return this->values[position];
}

double signal::get_max() const {
  double max = 0;
  for (int i = 0; i < this->size; i++) {
    if (fabs(this->values[i]) > max) max = fabs(this->values[i]);
  }
  return max;
}

double signal::tick() {
  if (this->position < 0 || this->position >= this->size) {
    if (!this->loop) return 0;
    this->position %= this->size;
  }
  return this->values[this->position++];
}

void signal::reset() {
  this->position = 0;
}

signal& operator+(const signal& sig1, const signal& sig2) {
  if (sig1.loop != sig2.loop) throw "Cannot add two signals with different loop values";

  int size = max(sig1.size, sig2.size);
  double* values = new double[size];

  for (int i = 0; i < size; i++) {
    values[i] = sig1.get_value(i) + sig2.get_value(i);
  }

  signal* output_signal = new signal(size, values);
  return *output_signal;
}

signal& operator-(const signal& sig1, const signal& sig2) {
  if (sig1.loop != sig2.loop) throw "Cannot add two signals with different loop values";

  int size = max(sig1.size, sig2.size);
  double* values = new double[size];

  for (int i = 0; i < size; i++) {
    values[i] = sig1.get_value(i) - sig2.get_value(i);
  }

  signal* output_signal = new signal(size, values);
  return *output_signal;
}

signal& operator*(const signal& sig1, const signal& sig2) {
  if (sig1.loop != sig2.loop) throw "Cannot add two signals with different loop values";

  int size = max(sig1.size, sig2.size);
  double* values = new double[size];

  for (int i = 0; i < size; i++) {
    values[i] = sig1.get_value(i) * sig2.get_value(i);
  }

  signal* output_signal = new signal(size, values);
  return *output_signal;
}

signal& operator/(const signal& sig1, const signal& sig2) {
  if (sig1.loop != sig2.loop) throw "Cannot add two signals with different loop values";

  int size = max(sig1.size, sig2.size);
  double* values = new double[size];

  for (int i = 0; i < size; i++) {
    values[i] = sig1.get_value(i) / sig2.get_value(i);
  }

  signal* output_signal = new signal(size, values);
  return *output_signal;
}

signal& derivative(const signal& sig) {
  int size = sig.size;
  double* values = new double[size];

  values[0] = (sig.get_value(1) - sig.get_value(0)) * SAMPLE_FREQ;
  values[size - 1] = (sig.get_value(size - 1) - sig.get_value(size - 2)) * SAMPLE_FREQ;
  for (int i = 1; i < size - 1; i++) {
    values[i] = (sig.get_value(i + 1) - sig.get_value(i - 1)) * (double)SAMPLE_FREQ / 2.0;
  }

  signal* output_signal = new signal(size, values, sig.loop);
  return *output_signal;
}

signal& normalize(const signal& sig) {
  int size =sig.size;
  double* nvalues = new double[size];
  double max = sig.get_max();
  for (int i=0; i<size; i++){
    nvalues[i]=sig.get_value(i)/max;
  }
  signal* output_signal = new signal(size, nvalues, sig.loop);
  return *output_signal;
}


signal& primitive(const signal& sig) {
  int size =sig.size;
  double* nvalues = new double[size];
  double current_sum = 0;
  nvalues[0] = 0;
  for (int i=1; i<size; i++){
    current_sum += (sig.get_value(i) + sig.get_value(i-1)) / (2 * SAMPLE_FREQ);
    nvalues[i] = current_sum;
  }
  signal* output_signal = new signal(size, nvalues, sig.loop);
  return *output_signal;
}

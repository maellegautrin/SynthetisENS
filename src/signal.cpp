#include "signal.h"
#include "SineWave.h"
#include <algorithm>
#include <iostream>

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
  if (position < 0 || position >= this->size) return this->loop ? this->values[position % size] : 0;
  return this->values[position];
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

#include "signal.h"
#include "SineWave.h"
#include <cmath>

#define PI 3.14159265358979323846
#define SAMPLE_RATE 44100

//RATE = SIZE * FREQ / SAMPLE_RATE

using namespace std;
using namespace synthetisens;

signal& operator+(signal& sig1, signal& sig2) {
  sum_signal* sum = new sum_signal(sig1, sig2);
  return *sum;
}

sum_signal::sum_signal(signal& sig1, signal& sig2) : sig1(sig1), sig2(sig2) {}

double sum_signal::tick() {
  return this->sig1.tick() + this->sig2.tick();
}


sin_signal::sin_signal(double freq) {
  this->sine.setFrequency(freq);
  this->time = 0;
}

double sin_signal::tick() {
  return this->sine.tick();
}

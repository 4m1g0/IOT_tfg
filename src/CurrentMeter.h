#include "Arduino.h"

const unsigned long sampleTime = 100000UL;
const unsigned long numSamples = 250UL;
const unsigned long sampleInterval = sampleTime/numSamples;
const int adc_zero = 729;

class CurrentMeter
{
public:
  CurrentMeter(uint8_t pin);
  float measure();

private:
  uint8_t _pin;
};

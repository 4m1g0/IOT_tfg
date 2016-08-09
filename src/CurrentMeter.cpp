#include "CurrentMeter.h"

CurrentMeter::CurrentMeter(uint8_t pin)
{
  _pin = pin;
}

float CurrentMeter::measure()
{
  unsigned long currentAcc = 0;
  unsigned int count = 0;
  unsigned long prevMicros = micros() - sampleInterval;

  while (count < numSamples)
  {
    if (micros() - prevMicros >= sampleInterval)
    {
      long adc_raw = (long) analogRead(_pin) - (long) adc_zero;
      currentAcc += (unsigned long) (adc_raw * adc_raw);
      ++count;
      prevMicros += sampleInterval;
    }
  }
  Serial.println(analogRead(_pin));
  return (float) sqrt((float)currentAcc/(float)numSamples) * (49.2 / 1024.0);
}

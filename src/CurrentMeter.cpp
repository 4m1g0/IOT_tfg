#include "CurrentMeter.h"

CurrentMeter::CurrentMeter(uint8_t pin)
{
  _pin = pin;
}

unsigned short CurrentMeter::measure()
{
  unsigned long currentAcc = 0;
  unsigned int count = 0;
  unsigned long prevMicros = micros() - sampleInterval;

  while (count < numSamples)
  {
    if (micros() - prevMicros >= sampleInterval)
    {
      unsigned long adc_raw = (long) analogRead(_pin) - (long) adc_zero;
      currentAcc += (unsigned long) (adc_raw * adc_raw);
      ++count;
      prevMicros += sampleInterval;
    }
  }

  float measure = (float) sqrt((float)currentAcc/(float)numSamples) * (40 / 1024.0);
  //Serial.println(analogRead(_pin));
  //Serial.print("float: ");
  //Serial.println((measure * 100.0f));
  return (unsigned short) (measure * 100.0f);
}

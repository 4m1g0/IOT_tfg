/*
 CurrentMeter.cpp - Class to measure currents
 Copyright (c) 2016 Oscar Blanco.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

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

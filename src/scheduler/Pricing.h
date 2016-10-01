/*
 NodeInfo.h - Class to store basic information and status of a node
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

#ifndef PRICING_H
#define PRICING_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "Schedule.h"

class Pricing
{
public:
  Pricing();

  // Update price array with online data
  bool update();

  // Returns the second within the interval in which we should turn it on
  unsigned long getBestTime(Schedule& schedule);

  uint32_t getPrice(unsigned long day, uint8_t hour);

//private:
  bool getTomorrowPrices(unsigned long date);
  bool getTodayPrices(unsigned long date);

    unsigned long lastUpdate;

  // 0-3 prices of the day before
  // 4-27 prices of the next day
  // it is updated every day at ~20:00
  uint32_t _price[28];
  unsigned long _date;
};
#endif

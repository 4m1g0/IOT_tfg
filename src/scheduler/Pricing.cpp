/*
 Pricing.cpp - Class to obtain and store energy prices
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

#include <Arduino.h>
#include "Pricing.h"
#include <ESP8266HTTPClient.h>
#include "../Clock.h"
#include "../config.h"
#include "ArduinoJson.h"

extern Config* config;

#define DEBUG_PRICING false

bool Pricing::getTomorrowPrices(unsigned long date)
{
  HTTPClient http;
  String url(config->pricing_address);
  unsigned long tomorrow = date+86400; // 86400s = 1 dia
  url.concat(Clock::getHumanDate(tomorrow));
  if (!http.begin(url.c_str()))
    return false;

  int httpCode = http.GET();
  if(httpCode != HTTP_CODE_OK)
    return false;

  String body = http.getString();
  http.end();

  DynamicJsonBuffer jsonBuffer;
  JsonArray& json = jsonBuffer.parseArray(body);

  if (!json.success() || json.size() != 24)
    return false;

  // We save remaining days from the day before (today)
  // as it must be at least 20:00 we only need lasts 3 prices
  for (int i = 0; i < 4; i++)
    _price[i] = _price[i+24];

  // 4-27 are the actual prices we are saving
  for (int i = 0; i < 24; i++)
    _price[i+3] = json.get(i);

  lastUpdate = date+86400; // 86400s = 1 dia
  _date = tomorrow;
  return true;
}

bool Pricing::getTodayPrices(unsigned long date)
{
  HTTPClient http;
  String url(config->pricing_address);
  url.concat(Clock::getHumanDate(date));

  if (!http.begin(url.c_str()))
    return false;

  int httpCode = http.GET();
  if(httpCode != HTTP_CODE_OK)
    return false;

  String body = http.getString();
  http.end();

  DynamicJsonBuffer jsonBuffer;
  JsonArray& json = jsonBuffer.parseArray(body);

  if (!json.success() || json.size() != 24)
    return false;

  // 4-27 are the actual prices we are saving
  // 0-3 are prices of the day before
  for (int i = 0; i < 24; i++)
    _price[i+3] = json.get<int>(i);

  lastUpdate = date;
  _date = date;
  return true;
}

bool Pricing::update()
{
  unsigned long today = Clock::getDayInSeconds();

  if (lastUpdate > today)
    return true; // we already have tomorrow prices (nothing to do)

  if (lastUpdate < today)
  {
    if (!getTodayPrices(today))
      return false;
  }

  // We should be able to obtain tomorrow prices after 20:00
  if ((Clock::getUnixTime() % 86400L) / 3600 > 20)
  {
    getTomorrowPrices(today);
  }

  return true;
}

Pricing::Pricing()
{
  lastUpdate = 0;
  if (!update())
    Serial.println("Failed to get prices");
}

uint32_t Pricing::getPrice(unsigned long day, uint8_t hour)
{
  if (_date == day)
  {
    // if we don't have data we use the same hour of the day before
    // this is the only way to schedule in an interval e.g.: 20:00 to 08:00
    // because at 20:00 we still don't have data of the next day and we can not
    // wait because startTime is comming
    return _price[(hour%24)+3];
  }

  if (day == _date-86400L) // We already have data of the next day
  {
    if (hour < 20) // we allready have erased that hour
      return 0;

    return _price[hour - 20];
  }

  return 0; // we don't have that data
}

uint8_t min(uint32_t* array, uint8_t len)
{
  uint32_t min = 4294967295; // max int32
  uint8_t minPos = 0;
  for(uint8_t i = 0; i < len; i++)
    if (array[i] < min)
    {
      min = array[i];
      minPos = i;
    }

  return minPos;
}

unsigned long Pricing::getBestTime(Schedule& schedule)
{
  if (schedule.startTime + schedule.interval < schedule.duration)
    return 0; // There is no enough time to run the schedule

  unsigned long today = Clock::getDayInSeconds();
  uint8_t interval = round(schedule.duration / 3600.0f);
  uint8_t start = round((schedule.startTime - today) / 3600.0f);
  uint8_t end = round((schedule.startTime + schedule.interval - today) / 3600.0f);

  if (!getPrice(today, start) || !getPrice(today, end))
    return 0; // we don't have enough data

  if (end - interval <= start)
    return schedule.startTime;

  #if DEBUG_PRICING
  Serial.print("start: ");
  Serial.println(start);
  Serial.print("end: ");
  Serial.println(end);
  Serial.print("interval: ");
  Serial.println(interval);
  #endif

  uint8_t spots = (end - interval) - start + 1;
  uint32_t integral[spots];
  for (uint8_t i = start; i <= end - interval; i++)
  {
    uint32_t sum = 0;
    for (uint8_t j=0; j < interval; j++)
      sum += getPrice(today, i+j);
    integral[i-start] = sum;
  }

  #if DEBUG_PRICING
  Serial.println("Integral");
  for (int i = 0; i < spots; i++)
  {
    Serial.print(integral[i]);
    Serial.print(",");
  }
  #endif

  uint8_t designatedHour = min(integral, spots)+start;
  unsigned long designatedSecond = designatedHour*3600 + today;
  // we have calculated everithing with 1h precision, so we need to adjust the time
  // due to round errors tu ensure the interval is exactly within the limits
  if ((designatedSecond < schedule.startTime)
    || (designatedHour == start
    && (schedule.startTime - Clock::getDayInSeconds()) / 3600.0f - float(start) < 0 // We have rounded up
    && getPrice(today, start-1) < getPrice(today, start+interval))) // prices is lower in the beginning
  {
    return schedule.startTime;
  }

  #if DEBUG_PRICING
  Serial.print("designatedSecond: ");
  Serial.println(Clock::getHumanDateTime(designatedSecond));
  Serial.print("start ");
  Serial.println(start);
  Serial.print("end: ");
  Serial.println(end);
  #endif


  if ((designatedSecond + schedule.duration > schedule.startTime + schedule.interval)
    || (designatedHour + interval == end
    && (schedule.startTime + schedule.interval - Clock::getDayInSeconds()) / 3600.0f - float(end) > 0 // We have rounded down
    && getPrice(today, end) < getPrice(today, end-interval)))
  {
    return schedule.startTime + schedule.interval - schedule.duration;
  }

  return designatedSecond;
}

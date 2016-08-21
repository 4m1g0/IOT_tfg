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

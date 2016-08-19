#include <Arduino.h>
#include "Pricing.h"
#include <ESP8266HTTPClient.h>
#include "../Clock.h"
#include "../config.h"
#include "ArduinoJson.h"

extern Config* config;

bool Pricing::getTomorrowPrices(unsigned long date)
{
  HTTPClient http;
  String url(config->pricing_address);
  url.concat(Clock::getHumanDate(date+86400)); // 86400s = 1 dia
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
    price[i] = price[i+24];

  // 4-27 are the actual prices we are saving
  for (int i = 0; i < 24; i++)
    price[i+3] = json.get(i);

  _lastUpdate = date+86400; // 86400s = 1 dia
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
    price[i+3] = json.get<int>(i);

  _lastUpdate = date;
  return true;
}

bool Pricing::update()
{
  unsigned long today = Clock::getDayInSeconds();

  if (_lastUpdate > today)
    return true; // we already have tomorrow prices (nothing to do)

  if (_lastUpdate < today)
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
  _lastUpdate = 0;
  update();
}

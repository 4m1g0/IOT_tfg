#include "Master.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

unsigned long Master::getTime()
{
  HTTPClient http;
  String url("http://");
  url.concat(WiFi.gatewayIP().toString());
  url.concat(":7001/clock");

  WiFi.mode(WIFI_STA);
  if (!http.begin(url))
    return 0;

  int httpCode = http.GET();
  if(httpCode != HTTP_CODE_OK)
    return 0;

  String body = http.getString();
  http.end();
  WiFi.mode(WIFI_AP_STA);

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(body);
  if (!json.success())
    return 0;

  return json.get<unsigned long>("t");
}

#ifndef CLIENTJSON_H
#define CLIENTJSON_H
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

class ClientJson : public HTTPClient
{
public:
  int sendRequest(const char * type, JsonObject& json);
  int sendRequest(const char * type, JsonArray& json);
};
#endif

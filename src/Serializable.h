#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H
#include <Arduino.h>
#include <ArduinoJson.h>

class Serializable
{
public:
  virtual JsonObject& toJson() = 0;
  virtual void fromJson(JsonObject& json) = 0;
};
#endif

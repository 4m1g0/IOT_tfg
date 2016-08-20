#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "../Serializable.h"

class Schedule : public Serializable<JsonObject&>
{
public:
  unsigned long startTime;
  uint32_t interval;
  uint32_t duration;
  uint32_t repeatEvery;

  void toJson(JsonObject& json);
  void fromJson(JsonObject& json);
};
#endif

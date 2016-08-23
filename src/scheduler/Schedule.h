#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "../Serializable.h"

class Schedule : public Serializable<JsonObject&>
{
public:
  uint8_t id;
  unsigned long startTime;
  uint32_t interval;
  uint32_t duration;
  uint32_t repeatEvery;
  unsigned long designatedTime;
  unsigned long lastRun;

  Schedule();
  void toJson(JsonObject& json);
  void fromJson(JsonObject& json);
};
#endif

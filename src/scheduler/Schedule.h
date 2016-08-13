#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "../Serializable.h"

class Schedule : public Serializable<JsonObject&>
{
public:
  unsigned long startTime;
  unsigned long endTime;
  unsigned long duration;
  unsigned long repeatEvery;

  void toJson(JsonObject& json);
  void fromJson(JsonObject& json);
};
#endif

#ifndef SCHEDILE_H
#define SCHEDILE_H

#include <Arduino.h>
#include <ArduinoJson.h>

class Schedule
{
public:
  unsigned long startTime;
  unsigned long endTime;
  unsigned long duration;
  unsigned long repeatEvery;

  JsonObject& toJson();
  void fromJson(JsonObject& json);
};
#endif

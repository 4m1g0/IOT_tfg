#include "Schedule.h"

void Schedule::toJson(JsonObject& json)
{
  json.set<unsigned long>("s", startTime);
  json.set<unsigned long>("e", endTime);
  json.set<unsigned long>("d", duration);
  json.set<unsigned long>("r", repeatEvery);
}

void Schedule::fromJson(JsonObject& json)
{
  startTime = json.get<unsigned long>("s");
  endTime = json.get<unsigned long>("e");
  duration = json.get<unsigned long>("d");
  repeatEvery = json.get<unsigned long>("r");
}

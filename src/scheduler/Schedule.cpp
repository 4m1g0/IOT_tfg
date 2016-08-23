#include "Schedule.h"

Schedule::Schedule()
: startTime(0)
, interval(0)
, duration(0)
, repeatEvery(0)
, lastRun(0)
, designatedTime(0)
, id(0)
{ }

void Schedule::toJson(JsonObject& json)
{
  json.set<uint8_t>("id", id);
  json.set<unsigned long>("s", startTime);
  json.set<uint32_t>("i", interval);
  json.set<uint32_t>("d", duration);
  json.set<uint32_t>("r", repeatEvery);
}

void Schedule::fromJson(JsonObject& json)
{
  startTime = json.get<unsigned long>("s");
  interval = json.get<uint32_t>("i");
  duration = json.get<uint32_t>("d");
  repeatEvery = json.get<uint32_t>("r");
}

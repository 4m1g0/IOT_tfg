#include "Schedule.h"

JsonObject& Schedule::toJson()
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root.set<unsigned long>("s", startTime);
  root.set<unsigned long>("e", endTime);
  root.set<unsigned long>("d", duration);
  root.set<unsigned long>("r", repeatEvery);

  return root;
}

void Schedule::fromJson(JsonObject& json)
{
  startTime = json.get<unsigned long>("s");
  endTime = json.get<unsigned long>("e");
  duration = json.get<unsigned long>("d");
  repeatEvery = json.get<unsigned long>("r");
}

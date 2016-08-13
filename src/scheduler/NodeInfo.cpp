#include "NodeInfo.h"
#include "Schedule.h"

int NodeInfo::addSchedule(Schedule schedule)
{
  if (schedules.size() >= MAX_SCHEDULES)
    return 0;

  schedules.push_back(schedule);
  return schedules.size();
}

void NodeInfo::toJson(Print &stream)
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root.set<short>("s", status);
  root.set<short>("t", type);
  root.set<unsigned long>("l", lastRun);

  JsonArray& array = root.createNestedArray("sh");

  for (std::vector<Schedule>::iterator it = schedules.begin(); it != schedules.end(); ++it)
    array.add(it->toJson());

  root.printTo(stream);
}

void NodeInfo::fromJson(JsonObject& json)
{
  status = (NodeStatus) json.get<short>("s");
  type = (NodeType) json.get<short>("t");
  lastRun = json.get<unsigned long>("l");

  JsonArray& array = json.get<JsonArray&>("sh");

  for (int i = 0; i < array.size(); i++)
  {
    Schedule schedule;
    schedule.fromJson(array.get<JsonObject&>(i));
    schedules.push_back(schedule);
  }
}

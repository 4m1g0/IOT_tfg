#include "NodeInfo.h"
#include "Schedule.h"

int NodeInfo::addSchedule(Schedule schedule)
{
  if (schedules.size() >= MAX_SCHEDULES)
    return 0;

  schedules.push_back(schedule);
  return schedules.size();
}

void NodeInfo::toJson(JsonObject& json)
{
  json.set<short>("s", status);
  json.set<short>("t", type);
  json.set<unsigned long>("l", lastRun);

  // REMOVE THIS!!!!
  JsonArray& arrayHistory = json.createNestedArray("h");
  history.toJson(arrayHistory);
  // REMOVE THIS!!!!

  JsonArray& arraySchedules = json.createNestedArray("sh");

  for (std::vector<Schedule>::iterator it = schedules.begin(); it != schedules.end(); ++it)
  {
    JsonObject& jsonObject = arraySchedules.createNestedObject();
    it->toJson(jsonObject);
  }
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

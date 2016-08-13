#include "NodeInfo.h"
#include "Schedule.h"

int NodeInfo::addSchedule(Schedule schedule)
{
  if (schedules.size() >= MAX_SCHEDULES)
    return 0;

  schedules.push_back(schedule);
  return schedules.size();
}

// in the normal method we don't return the history, to save memory (see toJsonDetails)
void NodeInfo::toJson(JsonObject& json)
{
  json.set<short>("s", status);
  json.set<short>("t", type);
  json.set<unsigned long>("l", lastRun);

  JsonArray& arraySchedules = json.createNestedArray("sh");

  for (std::vector<Schedule>::iterator it = schedules.begin(); it != schedules.end(); ++it)
  {
    JsonObject& jsonObject = arraySchedules.createNestedObject();
    it->toJson(jsonObject);
  }
}

// This methods includes the history, it is memory expensive, so most of the times is not needed
void NodeInfo::toJsonDetails(JsonObject& json)
{
  json.set<short>("s", status);
  json.set<short>("t", type);
  json.set<unsigned long>("l", lastRun);

  JsonArray& arrayHistory = json.createNestedArray("h");
  history.toJson(arrayHistory);

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

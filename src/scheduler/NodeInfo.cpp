#include "NodeInfo.h"
#include "Schedule.h"

/*NodeInfo::NodeInfo()
{

  // load from FS
}*/

int NodeInfo::addSchedule(Schedule schedule)
{
  if (schedules.size() >= MAX_SCHEDULES)
    return 0;

  uint8_t id = 0;
  while (true)
  {
    bool error = false;
    getSchedule(id, error);
    if (error)
      break;

    if (id >= 254)
      return 0;

    id++;
  }

  schedule.id = id;
  schedules.push_back(schedule);
  return schedules.size();
}

bool NodeInfo::modSchedule(Schedule schedule)
{
  for (std::vector<Schedule>::iterator it = schedules.begin(); it != schedules.end(); ++it)
    if (it->id == schedule.id)
    {
      it->designatedTime = schedule.designatedTime;
      it->duration = schedule.duration;
      it->interval = schedule.interval;
      it->startTime = schedule.startTime;
      it->lastRun = schedule.lastRun;
      it->repeatEvery = schedule.repeatEvery;
      return true;
    }

  return false;
}

Schedule NodeInfo::getSchedule(uint8_t id, bool& error)
{
  error = false;
  for (std::vector<Schedule>::iterator it = schedules.begin(); it != schedules.end(); ++it)
    if (it->id == id)
      return *it;

  error = true;
}

bool NodeInfo::delSchedule(uint8_t id)
{
  for (std::vector<Schedule>::iterator it = schedules.begin(); it != schedules.end(); ++it)
    if (it->id == id)
    {
      schedules.erase(it);
      return true;
    }
    return false;
}

// in the normal method we don't return the history, to save memory (see toJsonDetails)
void NodeInfo::toJson(JsonObject& json)
{
  json.set<short>("s", status);
  json.set<short>("t", type);
  json.set<unsigned long>("l", lastRun);

  JsonArray& arraySchedules = json.createNestedArray("sh");

  int i = 0;
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

  schedules.clear();
  for (int i = 0; i < array.size(); i++)
  {
    Schedule schedule;
    schedule.fromJson(array.get<JsonObject&>(i));
    schedules.push_back(schedule);
  }
}

void NodeInfo::on()
{
  Serial.println("poniendo on");
  status = NodeStatus::ON;
  digitalWrite(D0, HIGH);
}

void NodeInfo::off()
{
  Serial.println("poniendo off");
  status = NodeStatus::OFF;
  digitalWrite(D0, LOW);
}

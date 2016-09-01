#ifndef NODEINFO_H
#define NODEINFO_H

#include <Arduino.h>
#include "../Serializable.h"
#include "History.h"
#include <ArduinoJson.h>
#include <vector>
#include "Schedule.h"

const int MAX_SCHEDULES = 3;

enum NodeStatus { OFF = 0, ON };
enum NodeType
{
  SCHEDULABLE, // Can be started whenever is apropiate, but once is running it must keep on until finished
//  SCHEDULABLE_STOPABLE, // Can be started and stoped when needed
//  STOPABLE, // Can not be scheduled but, if running, it can be stoped if needed
  REAL_TIME // Can not be scheduled nor stoped
};

class NodeInfo : public Serializable<JsonObject&>
{
public:
  NodeInfo();
  void load();
  void save();
  History history;
  unsigned long lastRun;
  std::vector<Schedule> schedules;

  void on();
  void off();

  int addSchedule(Schedule schedule);
  void fromJson(JsonObject& json);
  void toJson(JsonObject& json); // doesn't return history
  void toJsonDetails(JsonObject& json); // do return history (memory expensive)
  void toJsonSchedules(JsonArray& json);
  Schedule getSchedule(uint8_t id, bool& error);
  bool modSchedule(Schedule schedule);
  bool delSchedule(uint8_t id);
  void setType(NodeType type);
  NodeType getType();
  bool isOn();
protected:
  NodeStatus _status;
  NodeType _type;
};
#endif

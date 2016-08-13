#include <Arduino.h>
#include "../Serializable.h"
#include "History.h"
#include <ArduinoJson.h>
#include <vector>
#include "Schedule.h"

const int MAX_SCHEDULES = 3;

enum NodeStatus { ON, OFF };
enum NodeType
{
  SCHEDULABLE, // Can be started whenever is apropiate, but once is running it must keep on until finished
  SCHEDULABLE_STOPABLE, // Can be started and stoped when needed
  STOPABLE, // Can not be scheduled but, if running, it can be stoped if needed
  REAL_TIME // Can not be scheduled nor stoped
};

class NodeInfo : public Serializable<JsonObject&>
{
public:
  NodeStatus status;
  NodeType type;
  History history;
  unsigned long lastRun;
  std::vector<Schedule> schedules;

  int addSchedule(Schedule schedule);
  void fromJson(JsonObject& json);
  void toJson(JsonObject& json); // doesn't return history
protected:

};

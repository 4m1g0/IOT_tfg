#include "NodeInfoTest.h"
#include "../scheduler/NodeInfo.h"

void NodeInfoTest::testAll()
{
  testNodeInfo();
}

void NodeInfoTest::testNodeInfo()
{
  Schedule schedule;
  schedule.duration = 123;
  schedule.endTime = 456;
  schedule.startTime = 67394;
  schedule.repeatEvery = 98765;

  NodeInfo nodeInfo;

  nodeInfo.history.addValue(123);
  nodeInfo.history.addValue(234);
  nodeInfo.history.addValue(56);

  nodeInfo.addSchedule(schedule);
  nodeInfo.addSchedule(schedule);
  nodeInfo.addSchedule(schedule);

  nodeInfo.status = OFF;
  nodeInfo.lastRun = 12333;
  nodeInfo.type = SCHEDULABLE;

  if (nodeInfo.addSchedule(schedule)) // max schedules per object
  {
    Serial.println("FAILED");
    return;
  }

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  nodeInfo.toJson(json);
  json.prettyPrintTo(Serial);
  Serial.println();
  Serial.println("===========================");
}

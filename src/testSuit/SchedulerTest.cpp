#include "SchedulerTest.h"
#include "../scheduler/Schedule.h"
#include "../scheduler/Scheduler.h"
#include "../scheduler/NodeInfo.h"
#include "../Clock.h"

void SchedulerTest::testAll()
{
  testUpdate();
  testScheduleNOW();
}

void SchedulerTest::testUpdate()
{
  Schedule schedule;
  schedule.duration = 1*3600; // 1h
  schedule.startTime = Clock::getUnixTime() - 86400;
  schedule.interval = 2*3600;
  schedule.repeatEvery = 86400;

  NodeInfo nodeInfo;
  nodeInfo.addSchedule(schedule);

  Serial.print("Before update: ");
  Serial.println(Clock::getHumanDateTime(nodeInfo.schedules.at(0).startTime));

  Pricing pricing;
  Scheduler::updateSchedules(pricing, nodeInfo);

  Serial.print("After update: ");
  Serial.println(Clock::getHumanDateTime(nodeInfo.schedules.at(0).startTime));

  Serial.print("designated: ");
  Serial.println(Clock::getHumanDateTime(nodeInfo.schedules.at(0).designatedTime));

  Serial.println("===========================");
}

void SchedulerTest::testScheduleNOW()
{
  Schedule schedule;
  schedule.duration = 20; // 1m
  schedule.startTime = Clock::getUnixTime() - 86400;
  schedule.interval = 20;
  schedule.repeatEvery = 86400;

  NodeInfo nodeInfo;
  nodeInfo.addSchedule(schedule);

  Serial.print("Before update: ");
  Serial.println(Clock::getHumanDateTime(nodeInfo.schedules.at(0).startTime));

  Pricing pricing;
  Scheduler::updateSchedules(pricing, nodeInfo);

  Serial.print("After update: ");
  Serial.println(Clock::getHumanDateTime(nodeInfo.schedules.at(0).startTime));

  Serial.print("designated: ");
  Serial.println(Clock::getHumanDateTime(nodeInfo.schedules.at(0).designatedTime));

  nodeInfo.status = NodeStatus::OFF;
  nodeInfo.type = NodeType::SCHEDULABLE;

  for (int i = 0; i < 4; i++)
  {
    Serial.print(i*10);
    Scheduler::schedule(nodeInfo);
    Serial.print(" ON?  ");
    Serial.println(nodeInfo.status == NodeStatus::ON ? "ON" : "OFF");
    delay(10000);
  }


  Serial.println("===========================");
}

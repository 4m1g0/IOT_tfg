/*
 SchedulerTest.cpp - Basic test suit for the Scheduler
 Copyright (c) 2016 Oscar Blanco.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


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

  nodeInfo.off();
  nodeInfo.setType(NodeType::SCHEDULABLE);

  for (int i = 0; i < 4; i++)
  {
    Serial.print(i*10);
    Scheduler::schedule(nodeInfo);
    Serial.print(" ON?  ");
    Serial.println(nodeInfo.isOn() ? "ON" : "OFF");
    delay(10000);
  }


  Serial.println("===========================");
}

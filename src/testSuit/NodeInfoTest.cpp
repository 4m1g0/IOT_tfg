/*
 NodeInfo.cpp - Basic test suit for NodeInfo
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
  schedule.interval = 456;
  schedule.startTime = 67394;
  schedule.repeatEvery = 98765;

  NodeInfo nodeInfo;

  nodeInfo.history.addValue(123);
  nodeInfo.history.addValue(234);
  nodeInfo.history.addValue(56);

  nodeInfo.addSchedule(schedule);
  nodeInfo.addSchedule(schedule);
  nodeInfo.addSchedule(schedule);

  nodeInfo.off();
  nodeInfo.lastRun = 12333;
  nodeInfo.setType(SCHEDULABLE);

  if (nodeInfo.addSchedule(schedule)) // max schedules per object
  {
    Serial.println("FAILED");
    return;
  }

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  nodeInfo.toJsonDetails(json);
  json.prettyPrintTo(Serial);
  Serial.println();
  Serial.println("===========================");
}

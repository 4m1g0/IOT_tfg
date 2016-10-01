/*
 ScheduleTest.cpp - Basic test suit for Schedule
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

#include "ScheduleTest.h"
#include "../scheduler/Schedule.h"

void ScheduleTest::testAll()
{
  testSchedule();
}

void ScheduleTest::testSchedule()
{
  Schedule schedule;
  schedule.duration = 123;
  schedule.interval = 456;
  schedule.startTime = 67394;
  schedule.repeatEvery = 98765;

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  schedule.toJson(json);
  json.prettyPrintTo(Serial);
  Serial.println();
  Serial.println("===========================");
}

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
  schedule.endTime = 456;
  schedule.startTime = 67394;
  schedule.repeatEvery = 98765;

  JsonObject& json = schedule.toJson();
  json.prettyPrintTo(Serial);
  Serial.println();
  Serial.println("===========================");
}

#include <Arduino.h>
#include "HistoryTest.h"
#include "ScheduleTest.h"
#include "NodeInfoTest.h"
#include "Test.h"

void Test::testAll()
{
  HistoryTest::testAll();
  //ScheduleTest::testAll();
  //NodeInfoTest::testAll();
}

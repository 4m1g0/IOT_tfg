
#include <Arduino.h>
#include "HistoryTest.h"
#include "../scheduler/History.h"
#include <ArduinoJson.h>

void HistoryTest::testAll()
{
  testRecord();

}

void HistoryTest::testRecord()
{
  Record<float> record;
  record.timestamp = 12345;
  record.value = 1.234f;

  JsonObject& json = record.toJson();
  json.prettyPrintTo(Serial);
  Serial.println();

  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root.set<long>("t", 12345);
  root.set<float>("v", 1.234f);
  if (root == json)
    Serial.println("PASSED");
  else
    Serial.println("FAILED");
  Serial.println("==========================");
}

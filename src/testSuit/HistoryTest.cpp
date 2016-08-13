#include <Arduino.h>
#include "HistoryTest.h"
#include "../scheduler/History.h"
#include <ArduinoJson.h>

void HistoryTest::testAll()
{
  //testRecord();
  testHistory();
}

void HistoryTest::testRecord()
{
  Record<float> record(12345, 1.234f);

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
  Serial.println("===========================");
}

void HistoryTest::testHistory()
{
  History history;

  history.addValue(1);
  history.addValue(2);
  history.addValue(3);
  history.addValue(4);
  history.addValue(5);
  history.addValue(6);

  DynamicJsonBuffer jsonBuffer;
  JsonArray& json = jsonBuffer.createArray();
  history.toJson(json);
  json.printTo(Serial);
  Serial.println();
  Serial.println("===========================");
}

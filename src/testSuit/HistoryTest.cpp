/*
 HistoryTest.cpp - Basic test suit for History
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

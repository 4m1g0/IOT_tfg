/*
 Master.cpp - Class that handles comunication with master node
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

#include "Master.h"
#include <ESP8266HTTPClient.h>
#include "ClientJson.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

unsigned long Master::getTime()
{
  /*HTTPClient http;
  String url("http://");
  url.concat(WiFi.gatewayIP().toString());
  url.concat(":7001/clock");

  WiFi.mode(WIFI_STA); // disable AP to avoid IP colisions
  if (!http.begin(url))
  {
    WiFi.mode(WIFI_AP_STA);
    return 0;
  }

  int httpCode = http.GET();
  if(httpCode != HTTP_CODE_OK)
  {
    WiFi.mode(WIFI_AP_STA);
    return 0;
  }

  String body = http.getString();
  http.end();
  WiFi.mode(WIFI_AP_STA);

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(body);
  if (!json.success())
    return 0;

  return json.get<unsigned long>("t");*/
}

void Master::schedule(NodeInfo* nodeInfo)
{
  /*ClientJson http;
  String url("http://");
  url.concat(WiFi.gatewayIP().toString());
  url.concat(":7001/schedule");

  WiFi.mode(WIFI_STA); // disable AP to avoid IP colisions
  if (!http.begin(url))
  {
    WiFi.mode(WIFI_AP_STA);
    return;
  }

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  nodeInfo->toJson(json);

  int httpCode = http.sendRequest("GET", json);
  if(httpCode != HTTP_CODE_OK)
  {
    WiFi.mode(WIFI_AP_STA);
    return;
  }

  String* body = new String(http.getString());
  http.end();
  WiFi.mode(WIFI_AP_STA);

  DynamicJsonBuffer jsonBuffer2;
  JsonObject& json2 = jsonBuffer2.parseObject(*body);
  delete body;

  if (!json2.success())
    return;

  JsonArray& array = json2.get<JsonArray&>("sh");

  for (uint16_t i = 0; i < array.size(); i++)
  {
    JsonObject& scheduleJson = array.get<JsonObject&>(i);
    Schedule schedule;
    schedule.fromJson(scheduleJson);
    nodeInfo->modSchedule(schedule);
  }*/
}

void Master::heartbeat(String name)
{
  /*ClientJson http;
  String url("http://");
  url.concat(WiFi.gatewayIP().toString());
  url.concat(":7001/heartbeat");

  WiFi.mode(WIFI_STA); // disable AP to avoid IP colisions
  if (!http.begin(url))
  {
    WiFi.mode(WIFI_AP_STA);
    return;
  }

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json.set<String>("n", name);

  http.sendRequest("POST", json);

  WiFi.mode(WIFI_AP_STA);*/
}

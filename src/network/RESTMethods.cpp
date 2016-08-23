#include "RESTMethods.h"
#include <ArduinoJson.h>
#include "../Clock.h"

void RESTMethods::clock(ServerJson& server)
{
  Serial.println("RESTMethods::clock");
  if (Clock::getUnixTime())
  {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json.set("t", Clock::getUnixTime());
    server.sendJson(200, json);
  }
  else
    server.send(503);
}

void RESTMethods::getInfo(ServerJson& server)
{
  Serial.println("RESTMethods::getInfo");
  if (!server.hasArg("id") || server.arg("id").compareTo(String(ESP.getChipId())) == 0)
  {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    nodeInfo->toJson(json);
    server.sendJson(200, json);
  }
  else
  {
    // ask other node
  }
}

void RESTMethods::getHistory(ServerJson& server)
{
  Serial.println("RESTMethods::getHistory");
  if (!server.hasArg("id") || server.arg("id").compareTo(String(ESP.getChipId())) == 0)
  {
    DynamicJsonBuffer jsonBuffer;
    JsonArray& json = jsonBuffer.createArray();
    nodeInfo->history.toJson(json);
    server.sendJson(200, json);
  }
  else
  {
    // ask other node
  }
}

void RESTMethods::addSchedule(ServerJson& server)
{
  Serial.println("RESTMethods::addSchedule");
  if (!server.hasArg("id") || server.arg("id").compareTo(String(ESP.getChipId())) == 0)
  {
    if (!server.hasArg("plain"))
    {
      Serial.println("Error 400");
      server.send(400);
      return;
    }

    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(server.arg("plain"));
    Schedule schedule;
    schedule.fromJson(json);
    nodeInfo->addSchedule(schedule);
    server.send(201);
  }
  else
  {
    // ask other node
  }
}

void RESTMethods::setState(ServerJson& server)
{
  Serial.println("RESTMethods::setState");
  if (!server.hasArg("id") || server.arg("id").compareTo(String(ESP.getChipId())) == 0)
  {
    if (!server.hasArg("plain"))
    {
      Serial.println("Error 400");
      server.send(400);
      return;
    }

    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(server.arg("plain"));
    if (json.get<bool>("s"))
    {
      Serial.println("on");
      nodeInfo->on();
    }
    else
    {
      Serial.println("off");
      nodeInfo->off();
    }

    server.send(200);
  }
  else
  {
    // ask other node
  }
}

void RESTMethods::modSchedule(ServerJson& server)
{
  Serial.println("RESTMethods::setState");
  if (!server.hasArg("id") || server.arg("id").compareTo(String(ESP.getChipId())) == 0)
  {
    if (!server.hasArg("plain"))
    {
      Serial.println("Error 400");
      server.send(400);
      return;
    }

    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(server.arg("plain"));
    Schedule schedule;
    schedule.fromJson(json);
    if (!nodeInfo->modSchedule(schedule))
    {
      server.send(404);
      return;
    }

    server.send(200);
  }
  else
  {
    // ask other node
  }
}

void RESTMethods::deleteSchedule(ServerJson& server)
{
  Serial.println("RESTMethods::setState");
  if (!server.hasArg("id") || server.arg("id").compareTo(String(ESP.getChipId())) == 0)
  {
    if (!server.hasArg("plain"))
    {
      Serial.println("Error 400");
      server.send(400);
      return;
    }

    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(server.arg("plain"));
    if (!nodeInfo->delSchedule(json.get<uint8_t>("id")))
    {
      server.send(404);
      return;
    }

    server.send(200);
  }
  else
  {
    // ask other node
  }
}

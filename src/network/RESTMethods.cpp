#include "RESTMethods.h"
#include <ArduinoJson.h>
#include "../Clock.h"
#include "../scheduler/Scheduler.h"
#include "Slave.h"

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
    if (nodeList.find(server.arg("id")) == nodeList.end() ) {
      server.send(503); // unavailable
      return;
    }

    IPAddress ip = nodeList[server.arg("id")].first;
    Slave slave(ip, 7001);
    slave.copyRequest(server);
  }
}

void RESTMethods::getschedules(ServerJson& server)
{
  if (!server.hasArg("id") || server.arg("id").compareTo(String(ESP.getChipId())) == 0)
  {
    DynamicJsonBuffer jsonBuffer;
    JsonArray& json = jsonBuffer.createArray();
    nodeInfo->toJsonSchedules(json);
    server.sendJson(200, json);
  }
  else
  {
    if (nodeList.find(server.arg("id")) == nodeList.end() ) {
      server.send(503); // unavailable
      return;
    }

    IPAddress ip = nodeList[server.arg("id")].first;
    Slave slave(ip, 7001);
    slave.copyRequest(server);
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
    if (nodeList.find(server.arg("id")) == nodeList.end() ) {
      server.send(503); // unavailable
      return;
    }

    IPAddress ip = nodeList[server.arg("id")].first;
    Slave slave(ip, 7001);
    slave.copyRequest(server);
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
    int id = nodeInfo->addSchedule(schedule);
    if (id < 0)
    {
      server.send(500);
      return;
    }
    String body = "{\"id\":";
    body.concat(id);
    body.concat("}");
    server.send(201, "application/json", body);
  }
  else
  {
    if (nodeList.find(server.arg("id")) == nodeList.end() ) {
      server.send(503); // unavailable
      return;
    }

    IPAddress ip = nodeList[server.arg("id")].first;
    Slave slave(ip, 7001);
    slave.copyRequest(server);
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
    if (json.is<uint8_t>("t"))
      nodeInfo->setType((NodeType)json.get<uint8_t>("t"));

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
    if (nodeList.find(server.arg("id")) == nodeList.end() ) {
      server.send(503); // unavailable
      return;
    }

    IPAddress ip = nodeList[server.arg("id")].first;
    Slave slave(ip, 7001);
    slave.copyRequest(server);
  }
}

void RESTMethods::modSchedule(ServerJson& server)
{
  Serial.println("RESTMethods::modSchedule");
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
    if (nodeList.find(server.arg("id")) == nodeList.end() ) {
      server.send(503); // unavailable
      return;
    }

    IPAddress ip = nodeList[server.arg("id")].first;
    Slave slave(ip, 7001);
    slave.copyRequest(server);
  }
}

void RESTMethods::deleteSchedule(ServerJson& server)
{
  Serial.println("RESTMethods::deleteSchedule");
  if (!server.hasArg("id") || server.arg("id").compareTo(String(ESP.getChipId())) == 0)
  {
    if (!server.hasArg("plain"))
    {
      // does it come in the url? DELETE /schedules/1
      if (server.uri().startsWith("/schedules/"))
      {
        String id = server.uri().substring(11);

        if (!nodeInfo->delSchedule(atoi(id.c_str())))
        {
          server.send(404);
          return;
        }

        server.send(200);
        return;
      }

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
    if (nodeList.find(server.arg("id")) == nodeList.end() ) {
      server.send(503); // unavailable
      return;
    }

    IPAddress ip = nodeList[server.arg("id")].first;
    Slave slave(ip, 7001);
    slave.copyRequest(server);
  }
}

void RESTMethods::schedule(ServerJson& server)
{
  Serial.println("RESTMethods::schedule");
  if (!server.hasArg("plain"))
  {
    Serial.println("Error 400");
    server.send(400);
    return;
  }

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(server.arg("plain"));

  NodeInfo nodeInfo;
  nodeInfo.fromJson(json);

  if (!Scheduler::schedule(nodeInfo))
  {
    server.send(400);
    return;
  }

  DynamicJsonBuffer jsonBuffer2;
  JsonObject& json2 = jsonBuffer2.createObject();
  nodeInfo.toJson(json2);
  server.sendJson(200, json);
}

void RESTMethods::heartbeat(ServerJson& server)
{
  Serial.println("RESTMethods::heartbeat");
  if (!server.hasArg("plain"))
  {
    Serial.println("Error 400");
    server.send(400);
    return;
  }

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(server.arg("plain"));
  String name = json.get<String>("n");
  nodeList[name].first = server.client().remoteIP();
  nodeList[name].second = Clock::getUnixTime();
  server.send(200);
}

void RESTMethods::getNodes(ServerJson& server)
{
  Serial.println("RESTMethods::getNodes");

  DynamicJsonBuffer jsonBuffer;
  JsonArray& json = jsonBuffer.createArray();
  typedef std::map<String, std::pair <IPAddress,unsigned long>>::iterator it_type;
  for(it_type iterator = nodeList.begin(); iterator != nodeList.end(); iterator++) {
    if (iterator->second.second + config->heartbeat_interval * 2 < Clock::getUnixTime())
    {
      nodeList.erase(iterator);
    }
    else
      json.add<String>(iterator->first);
  }
  server.sendJson(200, json);
}

/*
 NodeInfo.cpp - Class to store basic information and status of a node
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

#include "NodeInfo.h"
#include "Schedule.h"
#include "FS.h"
#include "../config.h"

extern Config* config;

NodeInfo::NodeInfo()
{
  _status = NodeStatus::OFF;
  _type = NodeType::SCHEDULABLE;
  lastRun = 0;

  load();
  if (_status == NodeStatus::ON)
    on();
}

void NodeInfo::load()
{
  File file = SPIFFS.open(config->nodeInfoFile, "r");
  if (!file)
  {
    Serial.println("Error while reading nodeInfo file. Creating new one...");
    return;
  }

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(file.readString());
  fromJson(json);
}

void NodeInfo::save()
{
  File file = SPIFFS.open(config->nodeInfoFile, "w");
  if (!file)
  {
    Serial.println("Error while writting nodeInfo file. Could not save...");
    return;
  }

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  toJsonDetails(json);
  json.printTo(file);
}

int NodeInfo::addSchedule(Schedule schedule)
{
  if (schedules.size() >= MAX_SCHEDULES)
    return -1;

  uint8_t id = 0;
  while (true)
  {
    bool error = false;
    getSchedule(id, error);
    if (error)
      break;

    if (id >= 254)
      return -1;

    id++;
  }

  schedule.id = id;
  schedules.push_back(schedule);
  save();
  return id;
}

bool NodeInfo::modSchedule(Schedule schedule)
{
  for (std::vector<Schedule>::iterator it = schedules.begin(); it != schedules.end(); ++it)
    if (it->id == schedule.id)
    {
      it->designatedTime = schedule.designatedTime;
      it->duration = schedule.duration;
      it->interval = schedule.interval;
      it->startTime = schedule.startTime;
      it->lastRun = schedule.lastRun;
      it->repeatEvery = schedule.repeatEvery;
      save();
      return true;
    }
  return false;
}

Schedule NodeInfo::getSchedule(uint8_t id, bool& error)
{
  error = false;
  for (std::vector<Schedule>::iterator it = schedules.begin(); it != schedules.end(); ++it)
    if (it->id == id)
      return *it;

  error = true;
}

bool NodeInfo::delSchedule(uint8_t id)
{
  for (std::vector<Schedule>::iterator it = schedules.begin(); it != schedules.end(); ++it)
    if (it->id == id)
    {
      schedules.erase(it);
      save();
      return true;
    }
    return false;
}

// in the normal method we don't return the history, to save memory (see toJsonDetails)
void NodeInfo::toJson(JsonObject& json)
{
  json.set<short>("s", _status);
  json.set<short>("t", _type);
  json.set<unsigned long>("l", lastRun);
}

void NodeInfo::toJsonSchedules(JsonArray& json)
{
  int i = 0;
  for (std::vector<Schedule>::iterator it = schedules.begin(); it != schedules.end(); ++it)
  {
    JsonObject& jsonObject = json.createNestedObject();
    it->toJson(jsonObject);
  }
}

void NodeInfo::toJsonDetails(JsonObject& json)
{
  json.set<short>("s", _status);
  json.set<short>("t", _type);
  json.set<unsigned long>("l", lastRun);

  JsonArray& arraySchedules = json.createNestedArray("sh");

  for (std::vector<Schedule>::iterator it = schedules.begin(); it != schedules.end(); ++it)
  {
    JsonObject& jsonObject = arraySchedules.createNestedObject();
    it->toJson(jsonObject);
  }
}

void NodeInfo::fromJson(JsonObject& json)
{
  _status = (NodeStatus) json.get<short>("s");
  _type = (NodeType) json.get<short>("t");
  lastRun = json.get<unsigned long>("l");

  JsonArray& array = json.get<JsonArray&>("sh");

  schedules.clear();
  for (int i = 0; i < array.size(); i++)
  {
    Schedule schedule;
    schedule.fromJson(array.get<JsonObject&>(i));
    schedules.push_back(schedule);
  }
}

void NodeInfo::on()
{
  Serial.println("Set ON");
  _status = NodeStatus::ON;
  digitalWrite(D5, LOW);
  save();
}

void NodeInfo::off()
{
  Serial.println("Set OFF");
  _status = NodeStatus::OFF;
  digitalWrite(D5, HIGH);
  save();
}

void NodeInfo::setType(NodeType type)
{
  _type = type;
}

NodeType NodeInfo::getType()
{
  return _type;
}

bool NodeInfo::isOn()
{
  return _status == NodeStatus::ON;
}

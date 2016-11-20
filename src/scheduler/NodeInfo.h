/*
 NodeInfo.h - Class to store basic information and status of a node
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

#ifndef NODEINFO_H
#define NODEINFO_H

#include <Arduino.h>
#include "../Serializable.h"
#include "History.h"
#include <ArduinoJson.h>
#include <vector>
#include "Schedule.h"

const int MAX_SCHEDULES = 3;

enum NodeStatus { OFF = 0, ON };
enum NodeType
{
  SCHEDULABLE, // Can be started whenever is apropiate, but once is running it must keep on until finished
//  SCHEDULABLE_STOPABLE, // Can be started and stoped when needed
//  STOPABLE, // Can not be scheduled but, if running, it can be stoped if needed
  REAL_TIME // Can not be scheduled nor stoped
};

class NodeInfo : public Serializable<JsonObject&>
{
public:
  NodeInfo();
  void load();
  void save();
  History history;
  unsigned long lastRun;
  std::vector<Schedule> schedules;

  void on();
  void off();

  int addSchedule(Schedule schedule);
  void fromJson(JsonObject& json);
  void toJson(JsonObject& json); 
  void toJsonDetails(JsonObject& json);
  void toJsonSchedules(JsonArray& json);
  Schedule getSchedule(uint8_t id, bool& error);
  bool modSchedule(Schedule schedule);
  bool delSchedule(uint8_t id);
  void setType(NodeType type);
  NodeType getType();
  bool isOn();
protected:
  NodeStatus _status;
  NodeType _type;
};
#endif

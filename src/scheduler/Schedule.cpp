/*
 Schedule.cpp - Class to store information about a schedule
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

#include "Schedule.h"

Schedule::Schedule()
: startTime(0)
, interval(0)
, duration(0)
, repeatEvery(0)
, lastRun(0)
, designatedTime(0)
, id(0)
{ }

void Schedule::toJson(JsonObject& json)
{
  json.set<uint8_t>("id", id);
  json.set<unsigned long>("s", startTime);
  json.set<uint32_t>("i", interval);
  json.set<uint32_t>("d", duration);
  json.set<uint32_t>("r", repeatEvery);
}

void Schedule::fromJson(JsonObject& json)
{
  id = json.get<uint8_t>("id");
  startTime = json.get<unsigned long>("s");
  interval = json.get<uint32_t>("i");
  duration = json.get<uint32_t>("d");
  repeatEvery = json.get<uint32_t>("r");
}

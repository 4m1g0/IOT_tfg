/*
 Scheduler.cpp - Class which contains shceduling algorithms
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


#include "Scheduler.h"
#include "Clock.h"
#include "../functions.h"
#include "../network/Master.h"

#define DEBUG_SCHEDULER false

bool Scheduler::updateSchedules(Pricing& pricing, NodeInfo& nodeInfo)
{
  if (!isMaster())
  {
    Master::schedule(&nodeInfo);
    return true;
  }

  unsigned long now = Clock::getUnixTime();
  bool changed = false;
  for (auto &schedule : nodeInfo.schedules)
  {
    // we already have a designated time or interval expired, so we can get next time
    if (schedule.repeatEvery > 0
        && ((schedule.designatedTime > schedule.startTime && schedule.designatedTime < schedule.startTime + schedule.interval)
            || schedule.startTime + schedule.interval < now))
    {
      schedule.startTime += schedule.repeatEvery * ceil(float(now - schedule.interval - schedule.startTime) / float(schedule.repeatEvery));
      changed = true;
    }

    #if DEBUG_SCHEDULER
    Serial.print("lastRun");
    Serial.println(schedule.lastRun);
    Serial.print("designatedtime");
    Serial.println(schedule.designatedTime);
    Serial.print("duration");
    Serial.println(schedule.duration);
    Serial.print("startime");
    Serial.println(schedule.startTime);
    Serial.print("now");
    Serial.println(now);
    Serial.print("startime");
    Serial.println(schedule.startTime);
    Serial.print("startime-now");
    Serial.println(long(schedule.startTime - now));
    #endif

      // It has already run or the designated time expiered
    if ((schedule.lastRun >= schedule.designatedTime || schedule.designatedTime + schedule.duration > now)
        // and we have a new plan to schedule
       && schedule.startTime > schedule.designatedTime
       // and there are only 30 minutes left to ensure we have data for the interval
       && long(schedule.startTime - now) < 1800)
    {
      if (isMaster())
        schedule.designatedTime = pricing.getBestTime(schedule);
      else
        {} //ask master for best time
      changed = true;
    }
  }

  if (changed)
    nodeInfo.save();

  return changed;
}

bool Scheduler::schedule(NodeInfo& nodeInfo)
{
  if (nodeInfo.getType() != NodeType::SCHEDULABLE)
    return 0;

  unsigned long now = Clock::getUnixTime();
  bool turnOff = true;

  if (nodeInfo.isOn())
  {
    for (auto &schedule : nodeInfo.schedules)
    {
      // If any schedule is still running don't turn off
      if (schedule.lastRun + schedule.duration > now)
        turnOff = false;
    }
  }

  if (!nodeInfo.isOn() || turnOff)
  {
    for (auto &schedule : nodeInfo.schedules)
    {
      if (schedule.designatedTime > schedule.lastRun
        && schedule.designatedTime <= now
        // we still have enough time to run it without overflowing the interval (with a 10m margin)
        && now + schedule.duration <= schedule.startTime + schedule.interval + 60*5)
      {
        schedule.lastRun = now;
        nodeInfo.on();
        turnOff = false;
      }
    }

    if (turnOff)
      nodeInfo.off();
  }
}

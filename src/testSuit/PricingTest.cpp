/*
 PricingTest.cpp - Basic test suit for Pricing
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


#include "PricingTest.h"
#include "../Clock.h"
#include "../scheduler/Pricing.h"

void PricingTest::testAll()
{
  testPricing();
  testBestTime();
}

void PricingTest::testPricing()
{
  Serial.println(Clock::getHumanDateTime(Clock::getUnixTime()));
  Pricing pricing;
  Serial.println(pricing.lastUpdate);
  for (int i = 0; i< 27; i++)
  {
    Serial.print(i);
    Serial.print("  ");
    Serial.println(pricing._price[i]);
  }

  Serial.println("===========================");
}

void PricingTest::testBestTime()
{
  // Lowest price of the day
  Schedule schedule;
  schedule.duration = 4*3600; // 1h
  schedule.startTime = Clock::getDayInSeconds() + 9*3600 + 60*30;
  schedule.interval = 6*3600 + 0*60*50;

  Pricing pricing;
  Serial.println(Clock::getHumanDateTime(pricing.getBestTime(schedule)));
  Serial.println("===========================");
}

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
  Serial.println(pricing._lastUpdate);
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
  schedule.endTime = Clock::getDayInSeconds() + 24*3600 + 0*60*50;

  Pricing pricing;
  Serial.println(Clock::getHumanDateTime(pricing.getBestTime(schedule)));
  Serial.println("===========================");
}

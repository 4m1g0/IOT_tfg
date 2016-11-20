/*
 Test.cpp - Basic class for unit tests
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
#include "ScheduleTest.h"
#include "NodeInfoTest.h"
#include "PricingTest.h"
#include "SchedulerTest.h"
#include "Test.h"

void Test::testAll()
{
  HistoryTest::testAll();
  ScheduleTest::testAll();
  NodeInfoTest::testAll();
  PricingTest::testAll();
  SchedulerTest::testAll();
}

/*
 RestMethods.h - Methods to handle http responses
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
#include "ServerJson.h"
#include "../scheduler/NodeInfo.h"
#include <map>
#include "../config.h"
#include "RestHandler.h"

extern NodeInfo* nodeInfo;
extern std::map <String, std::pair <IPAddress,unsigned long>> nodeList;
extern Config* config;

class RESTMethods : protected RestHandler
{
public:
  static void clock(ServerJson& server);
  static void getInfo(ServerJson& server);
  static void getHistory(ServerJson& server);
  static void addSchedule(ServerJson& server);
  static void setState(ServerJson& server);
  static void modSchedule(ServerJson& server);
  static void deleteSchedule(ServerJson& server);
  static void schedule(ServerJson& server);
  static void heartbeat(ServerJson& server);
  static void getNodes(ServerJson& server);
  static void getschedules(ServerJson& server);
private:

};

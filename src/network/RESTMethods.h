#include <Arduino.h>
#include "ServerJson.h"
#include "../scheduler/NodeInfo.h"
#include <map>
#include "../config.h"

extern NodeInfo* nodeInfo;
extern std::map <String, std::pair <IPAddress,unsigned long>> nodeList;
extern Config* config;

class RESTMethods
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

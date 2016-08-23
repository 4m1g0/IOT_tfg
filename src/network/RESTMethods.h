#include <Arduino.h>
#include "ServerJson.h"
#include "../scheduler/NodeInfo.h"

extern NodeInfo* nodeInfo;

class RESTMethods
{
public:
  static void clock(ServerJson& server);
  static void getInfo(ServerJson& server);
  static void getHistory(ServerJson& server);
  static void addSchedule(ServerJson& server);
private:

};

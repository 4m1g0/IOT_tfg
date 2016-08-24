#include <Arduino.h>
#include "../scheduler/NodeInfo.h"

class Master
{
public:
  static unsigned long getTime();
  static void schedule(NodeInfo* nodeInfo);
};

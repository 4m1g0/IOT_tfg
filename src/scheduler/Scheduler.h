#include <Arduino.h>
#include "NodeInfo.h"
#include "Pricing.h"

class Scheduler
{
public:
  // starts and stops
  static bool schedule(NodeInfo& nodeInfo);
  static bool updateSchedules(Pricing& pricing, NodeInfo& nodeInfo);
};

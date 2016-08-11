#include <Arduino.h>

const unsigned long NTP_TIMEOUT = 3000; // 3s

class Clock
{
public:
  static unsigned long getUnixTime();
  static void updateTime();

protected:
  static unsigned long _unixTime;
  static unsigned long _lastUpdateSeconds;
};

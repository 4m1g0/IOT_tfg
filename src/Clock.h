#include <Arduino.h>

const unsigned long NTP_TIMEOUT = 3000; // 3s

class Clock
{
public:
  static unsigned long getUnixTime();
  static String getHumanTime();
  static String getHumanDate();
  static String getHumanDateTime();
  static void updateTime();

protected:
  static unsigned long _unixTime;
  static unsigned long _lastUpdateSeconds;
};

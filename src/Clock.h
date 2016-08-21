#include <Arduino.h>

const unsigned long NTP_TIMEOUT = 3000; // 3s

class Clock
{
public:
  static unsigned long getUnixTime();
  static unsigned long getDayInSeconds();
  static String getHumanTime(unsigned long unixTime);
  static String getHumanDate(unsigned long unixTime);
  static String getHumanDateTime(unsigned long unixTime);
  static bool updateTime();

protected:
  static unsigned long _unixTime;
  static unsigned long _lastUpdate;
};

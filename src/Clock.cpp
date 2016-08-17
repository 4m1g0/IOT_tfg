#include <Arduino.h>
#include "Clock.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "config.h"
#include <time.h>

extern Config* config;
unsigned long Clock::_unixTime;
unsigned long Clock::_lastUpdateSeconds;

void Clock::updateTime()
{
  const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
  byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
  WiFiUDP udp;
  IPAddress timeServerIP;
  String ntpServerName = config->NTP_address;
  WiFi.hostByName(ntpServerName.c_str(), timeServerIP);
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.begin(2390);
  udp.beginPacket(timeServerIP, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();

  delay(1000);
  unsigned long startTime = millis();
  int cb;
  while((unsigned long)(millis() - startTime) < NTP_TIMEOUT && !cb)
    cb = udp.parsePacket();

  if (!cb)
  {
    Serial.println("WARNING: Unable to obtain updated time from NTP Server.");
    return;
  }

  udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
  udp.stop();

  //the timestamp starts at byte 40 of the received packet and is four bytes,
  // or two words, long. First, esxtract the two words:

  unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
  unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
  // combine the four bytes (two words) into a long integer
  // this is NTP time (seconds since Jan 1 1900):
  unsigned long secsSince1900 = highWord << 16 | lowWord;
  const unsigned long seventyYears = 2208988800UL;
  _unixTime = (secsSince1900 - seventyYears) + config->time_ofset;
  _lastUpdateSeconds = millis() / 1000;
}

unsigned long Clock::getUnixTime()
{
  unsigned long elapsed = (millis() / 1000) - _lastUpdateSeconds;
  return _unixTime + elapsed;
}

String Clock::getHumanDate()
{
  const time_t rawtime = (const time_t)Clock::getUnixTime();
  struct tm * dt;
  char timestr[30];
  char buffer [30];

  dt = localtime(&rawtime);

  String str;
  if (dt->tm_hour < 10) str.concat('0');
  str.concat(dt->tm_hour);
  str.concat(':');
  if (dt->tm_min < 10) str.concat('0');
  str.concat(dt->tm_min);
  str.concat(':');
  if (dt->tm_sec < 10) str.concat('0');
  str.concat(dt->tm_sec);

  return str;
}

String Clock::getHumanTime()
{
  const time_t rawtime = (const time_t)Clock::getUnixTime();
  struct tm * dt;
  char timestr[30];
  char buffer [30];

  dt = localtime(&rawtime);

  String str;
  if (dt->tm_mday < 10) str.concat('0');
  str.concat(dt->tm_mday);
  str.concat('-');
  if (dt->tm_mon < 10) str.concat('0');
  str.concat(dt->tm_mon);
  str.concat('-');
  str.concat(dt->tm_year + 1900);

  return str;
}

String Clock::getHumanDateTime()
{
  const time_t rawtime = (const time_t)Clock::getUnixTime();
  struct tm * dt;
  char timestr[30];
  char buffer [30];

  dt = localtime(&rawtime);

  String str;
  if (dt->tm_mday < 10) str.concat('0');
  str.concat(dt->tm_mday);
  str.concat('-');
  if (dt->tm_mon < 10) str.concat('0');
  str.concat(dt->tm_mon);
  str.concat('-');
  str.concat(dt->tm_year + 1900);
  str.concat(' ');
  if (dt->tm_hour < 10) str.concat('0');
  str.concat(dt->tm_hour);
  str.concat(':');
  if (dt->tm_min < 10) str.concat('0');
  str.concat(dt->tm_min);
  str.concat(':');
  if (dt->tm_sec < 10) str.concat('0');
  str.concat(dt->tm_sec);

  return str;
}

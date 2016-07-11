#include "functions.h"
#include "Arduino.h"
#include <ESP8266WiFi.h>

/**
* Gets the SSID of the master or empty string if I am the master
**/
String searchForMaster()
{
  String ssidPrefix = (*config)["ssid_prefix"];

  // The one with lowest chip id is master
  uint32_t currentChipId = ESP.getChipId();

  /* Scan for APs */
  int n = WiFi.scanNetworks();

  for (int i = 0; i < n; ++i) 
  {
    String current_ssid = WiFi.SSID(i);
    int index = current_ssid.indexOf(ssidPrefix);
    uint32_t target_chip_id = (current_ssid.substring(index + ssidPrefix.length())).toInt();
    if (index >= 0 && (target_chip_id < currentChipId))
      currentChipId = target_chip_id;
  }

  if (currentChipId != ESP.getChipId())
    return ssidPrefix + String(currentChipId);
  else
    return "";
}

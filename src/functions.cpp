#include "functions.h"
#include "Arduino.h"
#include <ESP8266WiFi.h>

/**
* Gets the SSID of the master or empty string if I am the master
**/
String searchForMaster()
{
  String ssidPrefix = config->ssid_prefix;

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

bool isMaster()
{
  return WiFi.status() == WL_CONNECTED && WiFi.SSID().compareTo(config->gateway_ssid) == 0;
}

void updateNetwork()
{
  Serial.println("Updating network");
  String MasterSSID = searchForMaster();

  String currentSSID = isMaster() ? "" : WiFi.SSID();

  if (WiFi.status() == WL_CONNECTED && currentSSID.compareTo(MasterSSID) == 0)
  {
    Serial.println("same network");
    return;
  }

  if (MasterSSID.length() > 0)
  {
    Serial.print("Connecting to master ");
    Serial.println(MasterSSID);
    String password = config->mesh_password;
    WiFi.begin(MasterSSID.c_str(), password.c_str());
  }
  else // I am the master, connect to gateway
  {
    Serial.print("Connecting to gateway ");
    String ssid = config->gateway_ssid;
    String password = config->gateway_password;
    Serial.println(ssid);
    WiFi.begin(ssid.c_str(), password.c_str());
  }

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i++ > 20) // 10s unable to connect
    {
      Serial.println("Unable to connect to any network, trying later...");
      return;
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void poolAllSlaves()
{
  String ssidPrefix = config->ssid_prefix;

  /* Scan for APs */
  int n = WiFi.scanNetworks();
  int chips = 0;
  for (int i = 0; i < n; ++i)
  {
    String current_ssid = WiFi.SSID(i);
    int index = current_ssid.indexOf(ssidPrefix);
    if (index >= 0)
      chips++;
  }

  Serial.println(chips);
}

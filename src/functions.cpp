/*
 functions.cpp - Basic low level functions
 Copyright (c) 2016 Oscar Blanco.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "functions.h"
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

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
  return true;
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
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    return;
  }

  WiFi.mode(WIFI_AP_STA);
  bool iamMaster = MasterSSID.length() <= 0;
  if (!iamMaster)
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
      WiFi.mode(WIFI_AP);
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

void handleConfig(ESP8266WebServer* server) {
  Serial.println("Config endpoint.");
  bool modified = false;

  for (uint8_t i=0; i<server->args(); i++){
    if (server->arg(i).length() == 0)
      continue;

    if (server->argName(i) == "ssid")
    {
      config->gateway_ssid = server->arg(i);
      modified = true;
    }
    else if (server->argName(i) == "pass")
    {
      config->gateway_password = server->arg(i);
      modified = true;
    }
    else if (server->argName(i) == "prefix")
    {
      config->ssid_prefix = server->arg(i);
      modified = true;
    }
    else if (server->argName(i) == "id")
    {
      config->network_id = server->arg(i);
      modified = true;
    }
    else if (server->argName(i) == "name")
    {
      config->name = server->arg(i);
      modified = true;
    }
  }

  if (!modified)
  {
    char content[] = "<html><head><title>Configuration</title></head><body><form method=\"GET\"><table style=\"margin:50px auto;width:350px;\"><tr><td align=\"center\" colspan=\"2\"><b>Configuration</b></td></tr><tr><td>Network prefix:</td><td><input type=\"text\" name=\"prefix\" /></td></tr><tr><td>Network id:</td><td><input type=\"text\" name=\"id\" /></td></tr><tr><td>GateWay SSID:</td><td><input type=\"text\" name=\"ssid\" /></td></tr><tr><td>Gateway password:</td><td><input type=\"password\" name=\"pass\" /></td></tr><tr><td>Node Name:</td><td><input type=\"text\" name=\"name\" /></td></tr><tr><td align=\"center\" colspan=\"2\"><input type=\"submit\" value=\"Send\" /></td></tr></table></form></body></html>";
    server->send(200, "text/html; charset=UTF-8", content);
  }
  else
  {
    config->saveConfig();
    server->send(200, "text/html; charset=UTF-8", "<html><head><title>Configuration</title></head><body><div style=\"font-weight:bolder;font-size:2em;margin:40px auto;width:20px;\">OK</div></body></html>");
    updateNetwork();
  }
}

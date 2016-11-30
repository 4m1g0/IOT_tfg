/*
 config.cpp basic config functionality
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

#include "config.h"
#include "FS.h"

const String Config::NTP_address = "0.europe.pool.ntp.org";
const String Config::pricing_address = "http://aalborgiot.4m1g0.com:9000/";
const String Config::nodeInfoFile = "nodeInfo.json";

String readFieldString(String rawConfig, String field)
{
  int index = rawConfig.indexOf(field);
  if (index < 0)
    return "";

  return rawConfig.substring(index + field.length()+1, rawConfig.indexOf(";", index));
}

int readFieldInt(String rawConfig, String field)
{
  String value = readFieldString(rawConfig, field);
  if (value.length() == 0)
    return -1;

  return value.toInt();
}

Config::Config(String file)
{
  m_file = file;
  readConfig();
}

void Config::readConfig()
{
  File file = SPIFFS.open(m_file, "r");
  if (!file)
  {
    Serial.println("Error while reading config file.");
    return;
  }
  String rawConfig = file.readString();

  ssid_prefix = readFieldString(rawConfig, SSID_PREFIX);
  name = readFieldString(rawConfig, NAME);
  proxy_url = readFieldString(rawConfig, PROXY_URL);
  mesh_password = readFieldString(rawConfig, MESH_PASSWORD);
  gateway_ssid = readFieldString(rawConfig, GATEWAY_SSID);
  gateway_password = readFieldString(rawConfig, GATEWAY_PASSWORD);
  network_interval = readFieldInt(rawConfig, NETWORK_INTERVAL);
  network_id = readFieldString(rawConfig, NETWORK_ID);
  type = readFieldInt(rawConfig, TYPE);
}

void Config::saveConfig()
{
  File file = SPIFFS.open(m_file, "w");

  if (!file)
  {
    Serial.println("Error while reading config file.");
    return;
  }

  file.println(String(SSID_PREFIX) + "=" + ssid_prefix + ";");
  file.println(String(NAME) + "=" + name + ";");
  file.println(String(PROXY_URL) + "=" + proxy_url + ";");
  file.println(String(MESH_PASSWORD) + "=" + mesh_password + ";");
  file.println(String(GATEWAY_SSID) + "=" + gateway_ssid + ";");
  file.println(String(GATEWAY_PASSWORD) + "=" + gateway_password + ";");
  file.println(String(NETWORK_INTERVAL) + "=" + network_interval + ";");
  file.println(String(NETWORK_ID) + "=" + network_id + ";");
  file.println(String(TYPE) + "=" + type + ";");
  file.close();
}

String Config::getRaw()
{
  saveConfig();
  File file = SPIFFS.open(m_file, "r");
  if (!file)
  {
    Serial.println("Error while reading config file.");
    return "";
  }
  return file.readString();
}

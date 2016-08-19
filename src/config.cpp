#include "config.h"
#include "FS.h"

const String Config::NTP_address = "0.europe.pool.ntp.org";
const String Config::pricing_address = "http://192.168.1.105:9000/";

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
  ree_url = readFieldString(rawConfig, REE_URL);
  proxy_url = readFieldString(rawConfig, PROXY_URL);
  mesh_password = readFieldString(rawConfig, MESH_PASSWORD);
  gateway_ssid = readFieldString(rawConfig, GATEWAY_SSID);
  gateway_password = readFieldString(rawConfig, GATEWAY_PASSWORD);
  network_inerval = readFieldInt(rawConfig, NETWORK_INTERVAL);
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
  file.println(String(REE_URL) + "=" + ree_url + ";");
  file.println(String(PROXY_URL) + "=" + proxy_url + ";");
  file.println(String(MESH_PASSWORD) + "=" + mesh_password + ";");
  file.println(String(GATEWAY_SSID) + "=" + gateway_ssid + ";");
  file.println(String(GATEWAY_PASSWORD) + "=" + gateway_password + ";");
  file.println(String(NETWORK_INTERVAL) + "=" + network_inerval + ";");
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

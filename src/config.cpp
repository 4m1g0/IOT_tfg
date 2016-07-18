#include "config.h"
#include "FS.h"

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
  delay(10000);
  Serial.println("hola");
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

  ssid_prefix = readFieldString(rawConfig, "ssid_prefix");
  ree_url = readFieldString(rawConfig, "ree_url");
  proxy_url = readFieldString(rawConfig, "proxy_url");
  mesh_password = readFieldString(rawConfig, "mesh_password");
  gateway_ssid = readFieldString(rawConfig, "gateway_ssid");
  gateway_password = readFieldString(rawConfig, "gateway_password");
  type = readFieldInt(rawConfig, "type");
}

void Config::saveConfig()
{

}

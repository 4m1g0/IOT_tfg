#ifndef config2
#define config2
#include "Arduino.h"

const char* const SSID_PREFIX = "ssid_prefix";
const char* const REE_URL = "ree_url";
const char* const PROXY_URL = "proxy_url";
const char* const MESH_PASSWORD = "mesh_password";
const char* const GATEWAY_SSID = "gateway_ssid";
const char* const GATEWAY_PASSWORD = "gateway_password";
const char* const TYPE = "type";

class Config
{
  String m_file;

public:
  String ssid_prefix;
  String ree_url;
  String proxy_url;
  String mesh_password;
  String gateway_ssid;
  String gateway_password;
  int type;

  Config(String file);
  void readConfig();
  void saveConfig();
  String getRaw();
};
#endif

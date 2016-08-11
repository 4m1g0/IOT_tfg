#include "Arduino.h"

const char* const SSID_PREFIX = "ssid_prefix";
const char* const REE_URL = "ree_url";
const char* const PROXY_URL = "proxy_url";
const char* const MESH_PASSWORD = "mesh_password";
const char* const GATEWAY_SSID = "gateway_ssid";
const char* const GATEWAY_PASSWORD = "gateway_password";
const char* const NETWORK_INTERVAL = "network_interval";
const char* const NETWORK_ID = "network_id";
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
  String network_id;
  int network_inerval;
  int type;

  // const values
  static const int measure_inerval = 60 * 1000; // 1m
  static const int updatetime_inerval = 10 * 60 * 1000; // 10m
  static const String NTP_address;

  Config(String file);
  void readConfig();
  void saveConfig();
  String getRaw();
};

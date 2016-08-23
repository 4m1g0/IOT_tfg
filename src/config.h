#ifndef FUNCTIONS_H
#define FUNCTIONS_H
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
  int network_interval;
  int type;

  // const values
  static const int measure_interval = 60 * 1000; // 1m
  static const int updatetime_interval = 10 * 60 * 60 * 1000; // 10h
  static const int schedule_interval = 2 * 60 * 1000; // 2m
  static const int pricingUpdate_interval = 5 * 60 * 1000; // 5m
  static const int time_ofset = 2 * 60 * 60; // 2h (GTM+2)
  static const String NTP_address;
  static const String pricing_address;

  Config(String file);
  void readConfig();
  void saveConfig();
  String getRaw();
};
#endif

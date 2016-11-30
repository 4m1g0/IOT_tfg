/*
 config.h - Basic config funtionality
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

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "Arduino.h"

const char* const SSID_PREFIX = "ssid_prefix";
const char* const NAME = "name";
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
  String name;
  String proxy_url;
  String mesh_password;
  String gateway_ssid;
  String gateway_password;
  String network_id;
  int network_interval;
  int type;

  // const values
  static const int measure_interval = 1 * 60 * 1000; // 1m
  static const int updatetime_interval = 10 * 60 * 60 * 1000; // 10h
  static const int schedule_interval = 2 * 60 * 1000; // 2m
  static const int pricingUpdate_interval = 5 * 60 * 1000; // 5m
  static const int heartbeat_interval = 1 * 60; //1m
  static const int firmwareUpdate_interval = 1 * 30 * 1000; // 15m
  static const int time_ofset = 2 * 60 * 60; // 2h (GTM+2)
  static const String NTP_address;
  static const String pricing_address;
  static const String nodeInfoFile;

  Config(String file);
  void readConfig();
  void saveConfig();
  String getRaw();
};
#endif

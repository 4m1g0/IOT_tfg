#include "Arduino.h"

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
};

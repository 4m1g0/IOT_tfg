
#include "FS.h"
#include <ESP8266WiFi.h>
#include "functions.h"

const char* CONFIG_PATH = "/global_config.conf";
unsigned long lastNetworkUpdate = 0;

Config* config;

void setup()
{
  Serial.begin(9600);
  Serial.println("");
  delay(10000);
  Serial.println("Mounting FS...");

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  config = new Config(CONFIG_PATH);

  WiFi.mode(WIFI_AP_STA);
  Serial.println((config->ssid_prefix + String(ESP.getChipId())).c_str());
  WiFi.softAP((config->ssid_prefix + String(ESP.getChipId())).c_str(), "12345678");

  updateNetwork();
  lastNetworkUpdate = millis();
}

void loop()
{
  if (millis() > config->network_inerval + lastNetworkUpdate)
  {
    Serial.println(config->network_inerval);
    Serial.println(lastNetworkUpdate);
    updateNetwork();
    lastNetworkUpdate = millis();
  }

  if (isMaster())
  {
    Serial.println("gola");
    poolAllSlaves();
  }
  else
  {
    // slave stuff
  }
}

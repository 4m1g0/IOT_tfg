
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
  updateNetwork();
  lastNetworkUpdate = millis();
}

void loop()
{
  if (millis() - config->network_inerval > lastNetworkUpdate)
  {
    updateNetwork();
    lastNetworkUpdate = millis();
  }

  if (isMaster())
  {
    // master stuff
  }
  else
  {
    // slave stuff
  }
}

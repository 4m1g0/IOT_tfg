
#include "FS.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "functions.h"

const char* CONFIG_PATH = "/global_config.conf";
unsigned long lastNetworkUpdate = 0;

Config* config;
ESP8266WebServer configServer(80); // Config server (web)
WiFiServer masterServer(7001);
WiFiServer slaveServer(7002);
WiFiServer remoteServer(8000);

void callback()
{
  handleConfig(&configServer);
}

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

  configServer.on("/", callback);
  configServer.begin();

  remoteServer.begin();
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

  // master and slave must listen for new config
  configServer.handleClient();

  if (isMaster())
  {
      slaveServer.stop(); // if closed does nothing
      if (masterServer.status() == CLOSED)
        masterServer.begin();




    Serial.println("gola");
    Serial.println(config->getRaw());
    //poolAllSlaves();
  }
  else
  {
    // slave stuff
  }
}

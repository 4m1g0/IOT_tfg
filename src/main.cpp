
#include "FS.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "functions.h"
#include "MeshServer.h"
#include "RemoteServer.h"

const char* CONFIG_PATH = "/global_config.conf";
unsigned long lastNetworkUpdate = 0;

Config* config;
ESP8266WebServer configServer(80); // Config server (web)
MeshServer meshServer(7001);
RemoteServer remoteServer(8000);

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


  remoteServer.addHandler([](WiFiClient client){ Serial.println(client.readString()); });
  configServer.on("/", [](){ handleConfig(&configServer); });
  meshServer.on("/", [](){ handleConfig(&meshServer); });
  configServer.begin();
  meshServer.begin();
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
    if (remoteServer.status() == CLOSED)
      remoteServer.begin();

    remoteServer.handleClient();


  }
  else
  {
    remoteServer.stop(); // if closed does nothing


    // slave stuff
  }

  meshServer.handleClient();
}

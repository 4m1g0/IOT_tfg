
#include "FS.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "functions.h"
#include "network/MeshServer.h"
#include "network/RemoteServer.h"
#include "CurrentMeter.h"
#include "Clock.h"
#include "testSuit/Test.h"

#define TESTSUIT true
const char* CONFIG_PATH = "/global_config.conf";
unsigned long lastNetworkUpdate = 0;
unsigned long lastMeasure = 0;
unsigned long lastTimeUpdate = 0;
const uint8_t ACT_PIN = D0;

Config* config;
ESP8266WebServer configServer(80); // Config server (web)
MeshServer meshServer(7001);
RemoteServer remoteServer(8000);

CurrentMeter currentMeter(A0);

void setup()
{
  pinMode(ACT_PIN, OUTPUT);
  digitalWrite(ACT_PIN, HIGH);
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

  if (isMaster())
  {
    Clock::updateTime();
    lastTimeUpdate = millis();
  }
  else
  {
    // Ask master for time
  }

  remoteServer.on("/", [](){ remoteServer.send(200, "text/html; charset=UTF-8", "hola"); });
  configServer.on("/", [](){ handleConfig(&configServer); });
  meshServer.on("/", [](){ handleConfig(&meshServer); });
  configServer.begin();
  meshServer.begin();
}

void loop()
{
  //Serial.println((unsigned long) 0 - 4294967287UL); in memoriam

#if TESTSUIT
  Test::testAll();
  delay(10000);
#endif

/*Serial.println("hola");
Serial.print("float: ");
float test1 = 907.02;
Serial.println(test1);
unsigned short test2 = test1;
Serial.print("short: ");
Serial.println(test2);
Serial.print("float*100: ");
float test3 = test1 * 100;
Serial.println(test3);
Serial.print("short*100: ");
unsigned short test4 = test3;
Serial.println(test4);*/
//

  // network update
  if ((unsigned long)(millis() - lastNetworkUpdate) > config->network_inerval)
  {
    Serial.println(config->network_inerval);
    Serial.println(lastNetworkUpdate);
    updateNetwork();
    lastNetworkUpdate = millis();
    Serial.println("network");
  }

  if ((unsigned long)(millis() - lastMeasure) > config->measure_inerval)
  {
    Serial.println("measure");
    // measure current and save it in the history
    lastMeasure = millis();
  }

  if ((unsigned long)(millis() - lastTimeUpdate) > config->updatetime_inerval)
  {
    Serial.println("clock");
    Clock::updateTime();
    lastTimeUpdate = millis();
  }


  // master and slave must listen for new config
  configServer.handleClient();

  if (isMaster())
  {
    //if (remoteServer.status() == CLOSED)
    //  remoteServer.begin();

    //remoteServer.handleClient();



    //Serial.println(currentMeter.measure());
    //digitalWrite(ACT_PIN, LOW);
    //delay(2000);
    //digitalWrite(ACT_PIN, HIGH);
    //delay(2000);

  }
  else
  {
    remoteServer.stop(); // if closed does nothing


    // slave stuff
  }

  meshServer.handleClient();
}

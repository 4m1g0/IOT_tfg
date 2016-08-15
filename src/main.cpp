#include <Arduino.h>
#include "FS.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "functions.h"
#include "network/MeshServer.h"
#include "network/RemoteServer.h"
#include "CurrentMeter.h"
#include "Clock.h"
#include "testSuit/Test.h"
#include "scheduler/NodeInfo.h"

#define TESTSUIT false
const char* CONFIG_PATH = "/global_config.conf";
unsigned long lastNetworkUpdate = 0;
unsigned long lastMeasure = 0;
unsigned long lastTimeUpdate = 0;
const uint8_t ACT_PIN = D0;

Config* config;
NodeInfo* nodeInfo;

ESP8266WebServer configServer(80); // Config server (web)
MeshServer meshServer(7001);
RemoteServer remoteServer(8000);

CurrentMeter currentMeter(A0);

void setup()
{
  pinMode(ACT_PIN, OUTPUT);
  digitalWrite(ACT_PIN, LOW);
  Serial.begin(9600);
  Serial.println("");
  delay(10000);
  Serial.println("Mounting FS...");

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  config = new Config(CONFIG_PATH);
  nodeInfo = new NodeInfo();

  // TEST REMOVE
  Schedule schedule1;
  schedule1.duration = 200;
  schedule1.endTime = 14057495;
  schedule1.startTime = 14057495;
  schedule1.repeatEvery = 100655;
  nodeInfo->addSchedule(schedule1);

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

  remoteServer.on("/", [](){ remoteServer.send(200, "text/html; charset=UTF-8", "It Works!"); });
  remoteServer.on("/info", [](){
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    nodeInfo->toJson(json);
    remoteServer.sendJson(200, json);
  });
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
    // measure current and save it in the history
    Serial.println("Measuring...");
    unsigned short current = currentMeter.measure();
    Serial.print("Current: "); Serial.println(current);
    nodeInfo->history.addValue(current);
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
    if (remoteServer.status() == CLOSED)
      remoteServer.begin();

    remoteServer.handleClient();



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

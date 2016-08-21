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
#include "scheduler/Pricing.h"
#include "network/RESTMethods.h"

#define TESTSUIT true
const char* CONFIG_PATH = "/global_config.conf";
unsigned long lastNetworkUpdate = 0;
unsigned long lastMeasure = 0;
unsigned long lastTimeUpdate = 0;
unsigned long lastSchedule = 0;
unsigned long lastPrincingUpdate = 0;
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

  WiFi.mode(WIFI_AP_STA);
  Serial.println((config->ssid_prefix + String(ESP.getChipId())).c_str());
  WiFi.softAP((config->ssid_prefix + String(ESP.getChipId())).c_str(), "12345678");

  updateNetwork();
  lastNetworkUpdate = millis();

  if (isMaster())
  {
    while (!Clock::updateTime())
    {
      Serial.println("Unable to get time. Retrying...");
      delay(20000);
    }
    lastTimeUpdate = millis();
  }
  else
  {
    // Ask master for time
  }

  remoteServer.on("/", [](){ remoteServer.send(200, "text/html; charset=UTF-8", "It Works!"); });
  meshServer.on("/clock", [](){ RESTMethods::clock(meshServer); });

  /*remoteServer.on("/info", HTTP_GET, [](){
    if (remoteServer.arg("id").compareTo(String(ESP.getChipId())) == 0)
    {
      // it's me
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();
      nodeInfo->toJson(json);
      remoteServer.sendJson(200, json);
    }
    else
    {
      // find ssid with id, connect, ask for the same and reply
    }



  });
  remoteServer.on("/info", HTTP_GET, [](){
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    nodeInfo->toJson(json);
    remoteServer.sendJson(200, json);
  });*/
  /*configServer.on("/", [](){ handleConfig(&configServer); });
  meshServer.on("/", [](){ handleConfig(&meshServer); });
  configServer.begin();
  meshServer.begin();*/
}

void loop()
{
  //Serial.println((unsigned long) 0 - 4294967287UL); in memoriam

#if TESTSUIT
  Test::testAll();
  delay(10000);
#endif
  /*Serial.print("HEap: ");
  Serial.println(ESP.getFreeHeap());
  if (!http.begin("http://192.168.1.105:9000/17-08-2016"))
    Serial.println("ERROR");
  Serial.println(ESP.getFreeHeap());
  int httpCode = http.GET();
  Serial.println(ESP.getFreeHeap());
  if(httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      Serial.println(ESP.getFreeHeap());
      // file found at server
      if(httpCode == HTTP_CODE_OK) {
          http.writeToStream(&Serial);
      }
  } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  delay(1000000);
  */
  //Serial.println(Clock::getHumanTime());
  //Serial.println(Clock::getHumanDate());
  /*Serial.println(Clock::getHumanDateTime(Clock::getUnixTime()));
  Pricing pricing;
  Serial.println(pricing._lastUpdate);
  for (int i = 0; i< 27; i++)
  {
    Serial.print(i);
    Serial.print("  ");
    Serial.println(pricing.price[i]);
  }
  delay(30000);*/


  /*// network update
  if ((unsigned long)(millis() - lastNetworkUpdate) > config->network_inerval)
  {
    Serial.println(config->network_inerval);
    Serial.println(lastNetworkUpdate);
    updateNetwork();
    lastNetworkUpdate = millis();
    Serial.println("network");
  }

  if ((unsigned long)(millis() - lastPrincingUpdate) > config->pricingUpdate_inerval)
  {
    // measure current and save it in the history
    Serial.println("Measuring...");
    unsigned short current = currentMeter.measure();
    Serial.print("Current: "); Serial.println(current);
    nodeInfo->history.addValue(current);
    lastMeasure = millis();
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

  if ((unsigned long)(millis() - lastSchedule) > config->schedule_inerval)
  {
    // power on and off depending on schedules
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

  meshServer.handleClient();*/
}

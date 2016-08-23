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

#define TESTSUIT false
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
RemoteServer remoteServer(8080);

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

  /*Schedule scheduleTest;
  scheduleTest.startTime = 123;
  scheduleTest.duration = 400;
  scheduleTest.interval = 800;
  nodeInfo->addSchedule(scheduleTest);*/

  WiFi.mode(WIFI_AP_STA);
  Serial.println((config->ssid_prefix + String(ESP.getChipId())).c_str());
  WiFi.softAP((config->ssid_prefix + String(ESP.getChipId())).c_str(), "12345678");

  updateNetwork();
  lastNetworkUpdate = millis();

  while (!Clock::updateTime())
  {
    Serial.println("Unable to get time. Retrying...");
    delay(20000);
  }
  lastTimeUpdate = millis();

  remoteServer.on("/", [](){ remoteServer.send(200, "text/html; charset=UTF-8", "It Works!"); });
  //remoteServer.on("/state", HTTP_PUT, [](){ RESTMethods::setState(remoteServer); });
  remoteServer.on("/schedules", HTTP_POST, [](){ RESTMethods::addSchedule(remoteServer); });
  //remoteServer.on("/schedules", HTTP_PUT, [](){ RESTMethods::modSchedule(remoteServer); });
  //remoteServer.on("/schedules", HTTP_DELETE, [](){ RESTMethods::deleteSchedule(remoteServer); });
  remoteServer.on("/info", HTTP_GET, [](){ RESTMethods::getInfo(remoteServer); });
  remoteServer.on("/history", HTTP_GET, [](){ RESTMethods::getHistory(remoteServer); });

  meshServer.on("/", [](){ meshServer.send(200, "text/html; charset=UTF-8", "It Works!"); });
  //meshServer.on("/schedules", HTTP_POST, [](){ RESTMethods::addSchedule(meshServer); });
  //meshServer.on("/schedules", HTTP_PUT, [](){ RESTMethods::modSchedule(meshServer); });
  //meshServer.on("/schedules", HTTP_DELETE, [](){ RESTMethods::deleteSchedule(meshServer); });
  //meshServer.on("/state", HTTP_PUT, [](){ RESTMethods::setState(meshServer); });
  meshServer.on("/clock", [](){ RESTMethods::clock(meshServer); });
  meshServer.on("/info", HTTP_GET, [](){ RESTMethods::getInfo(meshServer); });
  //meshServer.on("/schedule", HTTP_GET, [](){ RESTMethods::schedule(meshServer); });
  //meshServer.on("/history", HTTP_GET, [](){ RESTMethods::getHistory(meshServer); });
  meshServer.on("/info", HTTP_GET, [](){ RESTMethods::getInfo(meshServer); });

  configServer.on("/", [](){ handleConfig(&configServer); });

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
//Serial.println(Clock::getHumanDateTime(Clock::getUnixTime()));
//delay(1000);


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


  // network update
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
    lastPrincingUpdate = millis();
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

  meshServer.handleClient();
}

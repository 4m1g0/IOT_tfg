/*
 main.cpp - Main function, entry point for the application
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
#include "scheduler/Scheduler.h"
#include "network/Master.h"
#include <ESP8266httpUpdate.h>
#include <map>

#define TESTSUIT false
const char* CONFIG_PATH = "/global_config.conf";
unsigned long lastNetworkUpdate = 0;
unsigned long lastMeasure = 0;
unsigned long lastTimeUpdate = 0;
unsigned long lastSchedule = 0;
unsigned long lastPrincingUpdate = 0;
unsigned long lastFirmwareUpdate = 0;
unsigned long lastHeartbeat = 0;
const uint8_t ACT_PIN = D0;

Config* config;
NodeInfo* nodeInfo;
Pricing* pricing;
std::map <String, std::pair <IPAddress,unsigned long>> nodeList;

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

  WiFi.mode(WIFI_AP_STA);
  Serial.println((config->ssid_prefix + String(ESP.getChipId())).c_str());
  WiFi.softAP((config->ssid_prefix + String(ESP.getChipId())).c_str(), "12345678");

  updateNetwork();
  lastNetworkUpdate = millis();

  int i = 0;
  while (!Clock::updateTime() && i++ < 3)
  {
    Serial.println("Unable to get time. ");
    delay(5000);
  }
  lastTimeUpdate = millis();

  if (isMaster())
    pricing = new Pricing();

  remoteServer.on("/", [](){ remoteServer.send(200, "text/html; charset=UTF-8", "It Works!"); });
  remoteServer.on("/state", HTTP_PUT, [](){ RESTMethods::setState(remoteServer); });
  remoteServer.on("/schedules", HTTP_POST, [](){ RESTMethods::addSchedule(remoteServer); });
  remoteServer.on("/schedules", HTTP_PUT, [](){ RESTMethods::modSchedule(remoteServer); });
  remoteServer.on("/schedules", HTTP_DELETE, [](){ RESTMethods::deleteSchedule(remoteServer); });
  remoteServer.on("/schedules", HTTP_GET, [](){ RESTMethods::getschedules(remoteServer); });
  remoteServer.on("/info", HTTP_GET, [](){ RESTMethods::getInfo(remoteServer); });
  remoteServer.on("/history", HTTP_GET, [](){ RESTMethods::getHistory(remoteServer); });
  remoteServer.on("/nodes", HTTP_GET, [](){ RESTMethods::getNodes(remoteServer); });
  remoteServer.onNotFound([](){ RESTMethods::deleteSchedule(remoteServer); });

  meshServer.on("/", [](){ meshServer.send(200, "text/html; charset=UTF-8", "It Works!"); });
  meshServer.on("/schedules", HTTP_POST, [](){ RESTMethods::addSchedule(meshServer); });
  meshServer.on("/schedules", HTTP_PUT, [](){ RESTMethods::modSchedule(meshServer); });
  meshServer.on("/schedules", HTTP_DELETE, [](){ RESTMethods::deleteSchedule(meshServer); });
  meshServer.on("/schedules", HTTP_GET, [](){ RESTMethods::getschedules(meshServer); });
  meshServer.on("/state", HTTP_PUT, [](){ RESTMethods::setState(meshServer); });
  meshServer.on("/clock", [](){ RESTMethods::clock(meshServer); });
  meshServer.on("/info", HTTP_GET, [](){ RESTMethods::getInfo(meshServer); });
  meshServer.on("/schedule", HTTP_GET, [](){ RESTMethods::schedule(meshServer); });
  meshServer.on("/history", HTTP_GET, [](){ RESTMethods::getHistory(meshServer); });
  meshServer.on("/heartbeat", HTTP_POST, [](){ RESTMethods::heartbeat(meshServer); });
  meshServer.onNotFound([](){ RESTMethods::deleteSchedule(meshServer); });

  configServer.on("/", [](){ handleConfig(&configServer); });

  configServer.begin();
  meshServer.begin();
}

void loop()
{
  #if TESTSUIT
    Test::testAll();
    delay(10000);
  #endif

  yield();
  // network update
  if ((unsigned long)(millis() - lastNetworkUpdate) > config->network_interval)
  {
    updateNetwork();
    lastNetworkUpdate = millis();
    Serial.println("network");
  }

  if (isMaster() && (unsigned long)(millis() - lastPrincingUpdate) > config->pricingUpdate_interval)
  {
    Serial.println("Pricing update...");
    pricing->update();
    lastPrincingUpdate = millis();
  }

  if ((unsigned long)(millis() - lastMeasure) > config->measure_interval)
  {
    // measure current and save it in the history
    Serial.println("Measuring...");
    unsigned short current = currentMeter.measure();
    Serial.print("Current: "); Serial.println(current);
    nodeInfo->history.addValue(current);
    lastMeasure = millis();
  }

  if ((unsigned long)(millis() - lastTimeUpdate) > config->updatetime_interval)
  {
    Serial.println("clock");
    Clock::updateTime();
    lastTimeUpdate = millis();
  }


  if ((unsigned long)(millis() - lastSchedule) > config->schedule_interval)
  {
    // power on and off depending on schedules
    Scheduler::updateSchedules(*pricing, *nodeInfo);
    Scheduler::schedule(*nodeInfo);
    lastSchedule = millis();
  }

  if ((unsigned long)(millis() - lastFirmwareUpdate) > config->firmwareUpdate_interval)
  {
    Serial.println("Searching for firmware updates");
    t_httpUpdate_return ret = ESPhttpUpdate.update("https://4m1g0.com/update1.bin", "1", "FB 45 62 97 8D 0F 85 E1 5A E9 DB 87 70 35 E4 D1 04 75 87 6E");

    switch(ret) {
        case HTTP_UPDATE_FAILED:
            Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            break;

        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");
            break;

        case HTTP_UPDATE_OK:
            Serial.println("HTTP_UPDATE_OK");
            break;
    }
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

    // All slaves must keep master aware of their presence
    if ((unsigned long)(millis() - lastHeartbeat) > config->heartbeat_interval)
    {
      // Heartbeat
      Master::heartbeat(config->name);
      lastHeartbeat = millis();
      Serial.println("Heartbeat");
    }
  }

  meshServer.handleClient();
}

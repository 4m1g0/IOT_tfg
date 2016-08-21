#ifndef SERVERJSON_H
#define SERVERJSON_H
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

class ServerJson : public ESP8266WebServer
{
public:
  ServerJson(IPAddress addr, int port = 80);
  ServerJson(int port = 80);

  void sendJson(int code, JsonObject& json);
  void sendJson(int code, JsonArray& json);
};
#endif

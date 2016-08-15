#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

class RemoteServer : public ESP8266WebServer
{
public:
  RemoteServer(int port);

  uint8_t status();
  void handleClient();
  void stop();
  void close();
  void begin();
  void sendJson(int code, JsonObject& json);
  void sendJson(int code, JsonArray& json);

protected:
  int _port;
  bool _connect();
};

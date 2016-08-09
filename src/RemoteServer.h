#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <functional>

class RemoteServer;

class RemoteServer
{
public:
  typedef std::function<void(WiFiClient)> THandlerFunction;
  RemoteServer(int port);
  uint8_t status();
  void handleClient();
  void addHandler(THandlerFunction fn);
  void stop();
  void begin();

private:
  int _port;
  bool _connect();
  WiFiClient _client;
  THandlerFunction _handler;
};

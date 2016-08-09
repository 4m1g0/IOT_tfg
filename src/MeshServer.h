#include "Arduino.h"
#include <ESP8266WebServer.h>

class MeshServer : public ESP8266WebServer
{
public:
  MeshServer(int port);
  uint8_t status();
};

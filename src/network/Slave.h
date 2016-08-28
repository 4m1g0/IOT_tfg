#include <Arduino.h>
#include "ServerJson.h"

class Slave
{
public:
  Slave(IPAddress ip, uint16_t port);
  Slave(IPAddress ip);
  void copyRequest(ServerJson& server);

private:
  IPAddress _ip;
  uint16_t _port;
};

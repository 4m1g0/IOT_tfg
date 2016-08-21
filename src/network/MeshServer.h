#include "Arduino.h"
#include "ServerJson.h"

class MeshServer : public ServerJson
{
public:
  MeshServer(int port);
  uint8_t status();
};

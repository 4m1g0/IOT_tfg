#include "MeshServer.h"

MeshServer::MeshServer(int port) : ServerJson(port){ }

uint8_t MeshServer::status()
{
  return _server.status();
}

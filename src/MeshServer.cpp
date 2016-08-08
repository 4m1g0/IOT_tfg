#include "mesh_server.h"

MeshServer::MeshServer(int port) : ESP8266WebServer(port){ }

uint8_t MeshServer::status()
{
  return _server.status();
}

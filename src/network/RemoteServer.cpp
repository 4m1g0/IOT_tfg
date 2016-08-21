#include "RemoteServer.h"
#include "../config.h"

extern Config* config;

bool RemoteServer::_connect()
{
  if (_currentClient.connect(config->proxy_url.c_str(), _port))
  {
    _currentClient.println("ID: " + config->network_id);
    return true;
  }
  else
    return false;
}

RemoteServer::RemoteServer(int port)
: ServerJson(port)
{
  _port = port;
  _currentClient = WiFiClient();
}

void RemoteServer::begin()
{
  _connect();
  if(!_headerKeysCount)
    collectHeaders(0, 0);
}

void RemoteServer::stop()
{
  _currentClient.stop();
}

void RemoteServer::close()
{
  stop();
}

uint8_t RemoteServer::status()
{
  if (_currentClient.connected())
    return ESTABLISHED;
  else
    return CLOSED;
}

void RemoteServer::handleClient() {
  if (!_currentClient.connected() && !_connect())
    return;

  if(!_currentClient.available())
    return;

  if (!_parseRequest(_currentClient)) {
    _connect();
    return;
  }

  _contentLength = CONTENT_LENGTH_NOT_SET;
  _handleRequest();
}

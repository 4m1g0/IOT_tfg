#include "RemoteServer.h"
#include "config.h"

extern Config* config;

bool RemoteServer::_connect()
{
  if (_client.connect(config->proxy_url.c_str(), _port))
  {
    _client.println("ID: " + config->network_id);
    return true;
  }
  else
    return false;
}

RemoteServer::RemoteServer(int port)
{
  _port = port;
  _client = WiFiClient();
}

void RemoteServer::begin()
{
  _connect();
}

void RemoteServer::stop()
{
  _client.stop();
}

uint8_t RemoteServer::status()
{
  if (_client.connected())
    return ESTABLISHED;
  else
    return CLOSED;
}

void RemoteServer::addHandler(THandlerFunction fn)
{
  _handler = fn;
}

void RemoteServer::handleClient() {
  if (!_client.connected() && !_connect())
    return;

  if(!_client.available())
    return;

  if (!_handler)
    return;

  _handler(_client);
}

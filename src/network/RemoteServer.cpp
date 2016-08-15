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
: ESP8266WebServer(port)
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

void RemoteServer::sendJson(int code, JsonObject& json) {
    String header;
    const char * footer = "\r\n";
    _prepareHeader(header, code, "application/json", json.measureLength());
    _currentClient.write(header.c_str(), header.length());
    if(json.measureLength())
    {
      json.printTo(_currentClient);
      _currentClient.write(footer, 2);
    }

    json.printTo(Serial);
}

void RemoteServer::sendJson(int code, JsonArray& json) {
    String header;
    const char * footer = "\r\n";
    _prepareHeader(header, code, "application/json", json.measureLength());
    _currentClient.write(header.c_str(), header.length());
    if(json.measureLength())
    {
      json.printTo(_currentClient);
      _currentClient.write(footer, 2);
    }
}

/*
 RemoteServer.cpp - Implementarion of remote server
 Copyright (c) 2016 Oscar Blanco.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "RemoteServer.h"
#include "../config.h"

extern Config* config;

bool RemoteServer::_connect()
{
  if (_currentClient.connect(config->proxy_url.c_str(), _port))
  {
    lastConnection = millis();
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

void RemoteServer::handleClient()
{
  if (!_currentClient.connected() && !_connect())
    return;

  if ((unsigned long)(millis() - lastConnection) > reset_interval)
  {
    stop();
    delay(500);
    begin();
  }

  if(!_currentClient.available())
    return;

  if (!_parseRequest(_currentClient)) {
    _connect();
    return;
  }

  _contentLength = CONTENT_LENGTH_NOT_SET;
  _handleRequest();
}

/*
 RemoteServer.h - Main class of remote server
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


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ServerJson.h"
#include <ArduinoJson.h>

class RemoteServer : public ServerJson
{
public:
  RemoteServer(int port);

  uint8_t status();
  void handleClient();
  void stop();
  void close();
  void begin();
  void sendJson(int code, JsonObject& json);
  void sendJson(int code, JsonArray& json);

protected:
  int _port;
  bool _connect();
};

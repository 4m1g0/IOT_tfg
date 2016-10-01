/*
 ServerJson.cpp - Implemnetation of http server compatible with json
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

#include "ServerJson.h"

ServerJson::ServerJson(IPAddress addr, int port)
: ESP8266WebServer(addr, port)
{ }
ServerJson::ServerJson(int port)
: ESP8266WebServer(port)
{ }

void ServerJson::sendJson(int code, JsonObject& json) {
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

void ServerJson::sendJson(int code, JsonArray& json) {
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

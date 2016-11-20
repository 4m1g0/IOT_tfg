/*
 Slave.cpp - Class to comunicate with the slave nodes
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

#include "Slave.h"
#include "ClientJson.h"

Slave::Slave(IPAddress ip, uint16_t port)
{
  _ip = ip;
  _port = port;
}

Slave::Slave(IPAddress ip)
{
  Slave(ip, 7001);
}

void Slave::copyRequest(ServerJson& server)
{
  String method = "GET";
  if (server.method() == HTTP_POST) {
    method = "POST";
  } else if (server.method() == HTTP_DELETE) {
    method = "DELETE";
  } else if (server.method() == HTTP_OPTIONS) {
    method = "OPTIONS";
  } else if (server.method() == HTTP_PUT) {
    method = "PUT";
  } else if (server.method() == HTTP_PATCH) {
    method = "PATCH";
  }

  ClientJson client;
  client.begin(_ip.toString(), _port, server.uri());
  int statusCode = client.HTTPClient::sendRequest(method.c_str(), server.arg("plain"));
  server.send(statusCode, client.header("Content-type"), client.getString());
}

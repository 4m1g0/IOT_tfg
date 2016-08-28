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

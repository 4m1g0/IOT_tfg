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

    json.printTo(Serial);
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

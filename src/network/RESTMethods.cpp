#include "RESTMethods.h"
#include <ArduinoJson.h>
#include "../Clock.h"

void RESTMethods::clock(ServerJson& server)
{
  if (Clock::getUnixTime())
  {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json.set("t", Clock::getUnixTime());
    server.sendJson(200, json);
  }
  else
    server.send(503);
}

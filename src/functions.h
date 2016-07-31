#include "Arduino.h"
#include "config.h"
#include <ESP8266WebServer.h>

extern Config* config;

String searchForMaster();
void updateNetwork();
bool isMaster();
void poolAllSlaves();
void handleConfig(ESP8266WebServer* server);

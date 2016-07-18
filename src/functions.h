#include "Arduino.h"
#include "config.h"

extern Config* config;

String searchForMaster();
void updateNetwork();
bool isMaster();
void poolAllSlaves();

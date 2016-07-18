
#include "FS.h"
#include <ESP8266WiFi.h>
#include "functions.h"

const char* CONFIG_PATH = "/global_config.conf";

Config* config;

void setup()
{
	Serial.begin(9600);
  Serial.println("");
  delay(10000);
  Serial.println("Mounting FS...");

	if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

	config = new Config(CONFIG_PATH);

	String MasterSSID = searchForMaster();

	if (MasterSSID.length() > 0) // I am the master, connect to gateway
	{
		Serial.print("Connecting to master ");
	  Serial.println(MasterSSID);
		String password = config->mesh_password;
	  WiFi.begin(MasterSSID.c_str(), password.c_str());
	}
	else
	{
		Serial.print("Connecting to gateway ");
		String ssid = config->gateway_ssid;
		String password = config->gateway_password;
	  Serial.println(ssid);
	  WiFi.begin(ssid.c_str(), password.c_str());
	}

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


}

void loop()
{
	//int type = config.type;
	//Serial.println(type);
	delay(1000);
}

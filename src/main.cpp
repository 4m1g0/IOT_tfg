
#include "FS.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

const char* CONFIG_PATH = "/global_config.json";
const char* ssid     = "xx";
const char* password = "xx";

JsonObject* config;

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

	StaticJsonBuffer<200> jsonBuffer;
	File file = SPIFFS.open(CONFIG_PATH, "r");
	Serial.println("Reading config...");

	if (file)
	{
		JsonObject& root = jsonBuffer.parseObject(file.readString());
		config = &root;
	}
	else {
    Serial.println("Failed to read config");
    return;
  }

	Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

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
	int test = (*config)["test"];
	Serial.println(test);
	delay(1000);
}

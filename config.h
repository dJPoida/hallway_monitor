#ifndef CONFIG_H
#define CONFIG_H

#include <ArduinoJson.h>
#include "FS.h"

// WiFi Settings
// TODO: setup multiple WiFi SSIDs and Passwords
const char* ssid     = "ELDROIDZ";
const char* password = "pkmj4eva";
const int serverPort = 60691;

// IFTT Notification Endpoint
const boolean enableIftttNotification = false;
const char* iftttServer = "maker.ifttt.com";
const char* iftttEndpoint = "/trigger/maddie_spotted/with/key/c3nx5tMYazgNfI7rHTRS4y";

// Defaults
const boolean defaultLED_R = 255;
const boolean defaultLED_G = 255;
const boolean defaultLED_B = 255;

/**
 * Load the config.json from spiffs
 * @returns boolean true if the config could be loaded
 */
boolean loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  const char* serverName = json["serverName"];
  const char* accessToken = json["accessToken"];

  // Real world application would store these values in some variables for
  // later use.

  Serial.print("Loaded serverName: ");
  Serial.println(serverName);
  Serial.print("Loaded accessToken: ");
  Serial.println(accessToken);
  
  return true;
}

#endif

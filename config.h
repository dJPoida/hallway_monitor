#ifndef CONFIG_H
#define CONFIG_H

#include <ArduinoJson.h>
#include "FS.h"
#include "reset.h"


// WiFi Settings
const char* apssid          = "Hallway Monitor";  // The SSID when the device is running in Access Point mode
const char* mdnsAddress     = "hallway-monitor";  // The address that clients can use to connect to the device without the IP (i.e. http://hallway-monitor.local)
const char* networkHostname = "HallwayMonitor";   // The Hostname to display when other devices on the network want to identify this device
char*       ssid            = "";                 // The WiFi accesspoint to connect to (this is loaded from SPIFFS)
char*       password        = "";                 // The WiFi accesspoint password (this is loaded from SPIFFS)
int         serverPort      = 80;                 // The Port to run the server on (this is the default and after initial config is loaded from SPIFFS)

// IFTTT Details (If This Then That)
boolean iftttEnabled  = false;                // Whether the ifttt integration is enabled
char*   iftttServer   = "";                   // The ifttt Server address (typically "maker.ifttt.com")
char*   iftttEndpoint = "";                   // The ifttt Endpoint (something like "/trigger/maddie_spotted/with/key/yourkeyhere")

// Hardware Config
bool sensorEnabled = false;                   // Whether or not the sensor is enabled

// Defaults
const boolean defaultLED_R = 255;
const boolean defaultLED_G = 255;
const boolean defaultLED_B = 255;

// In the event that you stuff up your resister calculation (like I did) you can use these values between 0.0 and 1.0 to offset the RGB leds to get more natural colours
const double redBias = 0.2;
const double greenBias = 1.0;
const double blueBias = 1.0;



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

  const char* newSSID = json["ssid"];
  const char* newPassword = json["password"];
  const boolean newIFTTTEnabled = json["iftttEnabled"];
  const char* newIFTTTServer = json["iftttServer"];
  const char* newIFTTTEndpoint = json["iftttEndpoint"];

  ssid = const_cast<char*> (newSSID);
  password = const_cast<char*> (newPassword);
  iftttEnabled = newIFTTTEnabled;
  iftttServer = const_cast<char*> (newIFTTTServer);
  iftttEndpoint = const_cast<char*> (newIFTTTEndpoint);
  
  Serial.print("Config Loaded from SPIFFS: ");
  Serial.print(" - WiFi SSID: ");
  Serial.println(ssid);
  Serial.print(" - WiFi Password: ");
  Serial.println(password);
  Serial.print(" - iftttEnabled: ");
  Serial.println(iftttEnabled);
  Serial.print(" - ifttt Server: ");
  Serial.println(iftttServer);
  Serial.print(" - iftttEndpoint: ");
  Serial.println(iftttEndpoint);
  
  return true;
}


/**
 * Save the current configuration to SPIFFS
 */
bool saveConfig() {
  Serial.println("Writing settings to configuration file...");

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["ssid"] = ssid;
  json["password"] = password;
  json["iftttEnabled"] = iftttEnabled;
  json["iftttServer"] = iftttServer;
  json["iftttEndpoint"] = iftttEndpoint;

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  json.printTo(configFile);

  Serial.println("Successfully saved configuration file.");
  
  return true;
}

/**
 * Change the currrent Wifi Settings (triggered from the Configuration Webistes
 * @var char* newSSID the new WiFi SSID to save to the device
 * @var char* newPassword the new WiFi Password to save to the device
 */
void setWiFiSettings(const char* newSSID, const char* newPassword) {
    Serial.print("Configuring and saving new WiFi Hotspot details, SSID: '");
    Serial.print(newSSID);
    Serial.print("', Password: '");
    Serial.print(newPassword);
    Serial.print("'...");

    ssid = const_cast<char*> (newSSID);
    password = const_cast<char*> (newPassword);

    // Save the updated config.
    saveConfig();

    Serial.println(" done.");
    Serial.println("restarting...");

    // Reset the device
    resetDevice();
}

#endif

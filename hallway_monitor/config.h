#ifndef CONFIG_H
#define CONFIG_H

#include <ArduinoJson.h>
#include "FS.h"
#include "reset.h"


// WiFi Settings
const char* apssid          = "Hallway Monitor";  // The SSID when the device is running in Access Point mode
const char* mdnsAddress     = "hallway-monitor";  // The address that clients can use to connect to the device without the IP (i.e. http://hallway-monitor.local)
const char* networkHostname = "HallwayMonitor";   // The Hostname to display when other devices on the network want to identify this device
String      ssid            = "";                 // The WiFi accesspoint to connect to (this is loaded from SPIFFS)
String      password        = "";                 // The WiFi accesspoint password (this is loaded from SPIFFS)
int         serverPort      = 80;                 // The Port to run the server on (this is the default and after initial config is loaded from SPIFFS)

// IFTTT Details (If This Then That)
boolean     iftttEnabled   = false;               // Whether the ifttt integration is enabled
String      iftttServer    = "";                  // The ifttt Server address (typically "maker.ifttt.com")
String      iftttEndpoint  = "";                  // The ifttt Endpoint (something like "/trigger/maddie_spotted/with/key/yourkeyhere")

// Hardware Config
bool        sensorEnabled = false;                // Whether or not the sensor is enabled
bool        nightlightOn = false;                 // Whether the nightlight is currently on or not

// LED States (these could be bytes but there's issues storing to JSON)
int         LED_R = 255;                          // The R Color Value of the Nightlight
int         LED_G = 255;                          // The G Color Value of the Nightlight
int         LED_B = 255;                          // The B Color Value of the Nightlight

// In the event that you stuff up your resister calculation (like I did) you can use these values between 0.0 and 1.0 to offset the RGB leds to get more natural colours
const double redBias = 0.2;
const double greenBias = 1.0;
const double blueBias = 1.0;

// Globals
bool writingConfig = false;                       // Keeps track of whether the config is being written at the moment
const long lazyWriteDelay = 1000;                 // Wait this many MS to lazy write the config (useful to ensure we don't write too much consecutively)
bool lazyWriteConfigFlag = false;                 // Flag to indicate that the config needs to be written, just not right now.
long lastLazyWriteConfigDebounce = 0;             // Last time something wanted to lazy write to the config


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

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  //Close the file before we corrupt it.
  configFile.close();

  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  ssid = json.get<String>("ssid");
  password = json.get<String>("password");
  iftttEnabled = json.get<bool>("iftttEnabled");
  iftttServer = json.get<String>("iftttServer");
  iftttEndpoint = json.get<String>("iftttEndpoint");
  nightlightOn = json.get<bool>("nightlightOn");
  sensorEnabled = json.get<bool>("sensorEnabled");
  LED_R = json.get<byte>("r");
  LED_G = json.get<byte>("g");
  LED_B = json.get<byte>("b");
    
  Serial.println("Config Loaded from SPIFFS: ");
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
  Serial.print(" - Night Light On: ");
  Serial.println(nightlightOn);
  Serial.print(" - Night Light R: ");
  Serial.print(LED_R);
  Serial.print(" G: ");
  Serial.print(LED_G);
  Serial.print(" B: ");
  Serial.println(LED_B);
  Serial.print(" - Sensor Enabled: ");
  Serial.println(sensorEnabled);
  
  return true;
}


/**
 * Save the current configuration to SPIFFS
 */
bool saveConfig() {
  // Wait for any current write operations to finish
  while (writingConfig) {
    delay(1);
  };
  
  lazyWriteConfigFlag = false;
  writingConfig = true;
  Serial.println("Writing settings to configuration file...");

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["ssid"] = ssid ? ssid : "";
  json["password"] = password ? password : "";
  json["iftttEnabled"] = iftttEnabled;
  json["iftttServer"] = iftttServer;
  json["iftttEndpoint"] = iftttEndpoint;
  json["nightlightOn"] = nightlightOn;
  json["sensorEnabled"] = sensorEnabled;
  json["r"] = LED_R;
  json["g"] = LED_G;
  json["b"] = LED_B;

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  // Debug
  //String jsonData;
  //json.printTo(jsonData);
  //Serial.print("JSON: ");
  //Serial.println(jsonData);
  
  json.printTo(configFile);
  configFile.close();
  
  writingConfig = false;
  
  return true;
}



/**
 * Change the currrent Wifi Settings (triggered from the Configuration Webiste)
 * @var String newSSID the new WiFi SSID to save to the device
 * @var String newPassword the new WiFi Password to save to the device
 */
void setWiFiSettings(String newSSID, String newPassword) {
    ssid = newSSID;
    password = newPassword;

    Serial.print("Configuring and saving new WiFi Hotspot details, SSID: '");
    Serial.print(ssid);
    Serial.print("', Password: '");
    Serial.print(password);
    Serial.println("'...");

    // Save the updated config.
    saveConfig();

    Serial.println("restarting...");

    // Reset the device
    flagReset();
}



/**
 * Change the currrent IFTTT Push Settings (triggered from the Configuration Webiste)
 * @var boolean newIFTTTEnabled whether push to the IFTTT server is enabled
 * @var String newIFTTTServer the new IFTTT Server address
 * @var String newIFTTTEndpoint the new IFTTT Endpoint to push to 
 */
void setIFTTTSettings(boolean newIFTTTEnabled, String newIFTTTServer, String newIFTTTEndpoint) {
    iftttEnabled = newIFTTTEnabled;
    iftttServer = newIFTTTServer;
    iftttEndpoint = newIFTTTEndpoint;

    Serial.print("Configuring and saving new IFTTT configuration, iftttEnabled: '");
    Serial.print(iftttEnabled);
    Serial.print("', iftttServer: '");
    Serial.print(iftttServer);
    Serial.print("', iftttEndpoint: '");
    Serial.print(iftttEndpoint);
    Serial.println("'...");

    // Save the updated config.
    saveConfig();
}



/**
 * Indicate that the config file needs to be written, but not right now.
 */
void lazyWriteConfig() {
  lazyWriteConfigFlag = true;
  lastLazyWriteConfigDebounce = millis();
}



/**
 * Checks to see if something is wanting to write the config to SPIFFS lazily 
 * (i.e. at the next opportunity)
 */
void checkLazyWriteConfig() {
  if (lazyWriteConfigFlag) {
    if (millis() - lastLazyWriteConfigDebounce > lazyWriteDelay) {
      Serial.println("Lazy write config triggered");
      saveConfig();
    }
  }
}

#endif

/**
 * Hallway Monitor
 * 
 * IFTT Sensor Alarm and WiFi enabled RGB Night Light
 * Author: Peter Eldred
 * Date: 2018-11
 * 
 * Dependencies:
 *  - ESP8266 Sketch Data Upload Tool (http://esp8266.github.io/Arduino/versions/2.0.0/doc/filesystem.html)
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "config.h"
#include "pins.h"
#include "wifi.h"
#include "webServer.h"
#include "sensor.h"

// Globals
const char* ver = "1.0.0";



/**
 * Setup
 */
void setup() {
  delay(1000);
  
  // Serial Initialisation
  Serial.begin(115200);
  Serial.println("");
  Serial.print("Hallway Monitor v");
  Serial.println(ver);

  // Start the SPI Flash Files System
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
  Serial.println("SPIFFS initialised.");

  // Load the config from the onboard SPI File System
  if (!loadConfig()) {
    Serial.println("Failed to load configuration from SPIFFS");
    Serial.print("Creating new Config File... ");
    if (!saveConfig()) {
      Serial.println("Failed to create new config file on SPIFFS.");
      return;
    }
    Serial.println("done.");
  }

  // Give SPIFFS a tick to catch up
  delay(250);
  
  // Initialise the LED
  if (!initLED()) {
    Serial.println("Failed to initialise the LED");
    return;
  }

  // Initialise the Reset Capability
  if (!initReset()) {
    Serial.println("Failed to initialise the Reset function");
    return;
  }

  // Initialise the WiFi
  if (!initWifi()) {
    Serial.println("Failed to initialise the WiFi.");

    // Failed to initialise the WiFi - Begin broadcasting a hotspot
    initAccessPointMode();
  }

  // Initialise the Web Server
  if (!initWebServer()) {
    Serial.println("Failed to initialise the Web Server");
    return;
  }

  // Don't initialise these things if we're in hotSpotMode
  if (!accessPointMode) {
    // Initialise the Sensor
    if (!initSensor()) {
      Serial.println("Failed to initialise the PIR Sensor");
      return;
    }
  }
}



/**
 * Application Loop
 */
void loop() {
  // Has something requested a reset?
  if (resetFlag) {
    resetDevice();
  }

  // Don't perform these operations in hotSpotMode
  if (!accessPointMode) {
    checkSensor();
  } else {
    // Flash the LEDs to indicate the device is in configuration mode
    handleFlashAPModeLED();
  }

  // Check so see if something wants to write to the config
  checkLazyWriteConfig();

  delay(1);
}

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
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "config.h"
#include "pins.h"
#include "wifi.h"
#include "webServer.h"

// Globals
const char* ver = "1.0.0";

// States
bool alarm_triggered = false;

/**
 * Fired when the alarm interrupt is triggered
 */
void onSensorTripped()
{
  if (!alarm_triggered) {
    alarm_triggered = true;
  }
}



/**
 * Setup
 */
void setup() {
  // Serial Initialisation
  Serial.begin(115200);
  Serial.println("");
  Serial.print("Hallway Monitor v");
  Serial.println(ver);

  // Start the SPI Flash Files System
  SPIFFS.begin();

  // Initialise the LED
  initLED();

  // Initialise the WiFi
  initWifi();

  // Initialise the Server
  initServer();

  // Connect the Sensor Interrupt
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR), onSensorTripped, RISING);
}



/**
 * Application Loop
 */
void loop() {
  // Check to see if the alarm has been triggered
  if (alarm_triggered) {
    sendTriggerToIFTTT();
  }

  server.handleClient();

  delay(10);
}



/**
 * 
 */
void sendTriggerToIFTTT() {
  Serial.println("Sensor Tripped - Sending Trigger to IFTTT");
  makeIFTTTRequest();
  alarm_triggered = false;
  delay(10000);
}



// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest() {
  // Make sure the WiFi is connected
  if (initWifi()) {
    Serial.print("Connecting to "); 
    Serial.print(iftttServer);
    
    WiFiClient client;
    int retries = 5;
    while(!!!client.connect(iftttServer, 80) && (retries-- > 0)) {
      Serial.print(".");
    }
    Serial.println();
    if(!!!client.connected()) {
       Serial.println("Failed to connect, going back to sleep");
    }
    
    Serial.print("Request resource: "); 
    Serial.println(iftttEndpoint);
    client.print(String("GET ") + iftttEndpoint + 
                    " HTTP/1.1\r\n" +
                    "Host: " + iftttServer + "\r\n" + 
                    "Connection: close\r\n\r\n");
                    
    int timeout = 5 * 10; // 5 seconds             
    while(!!!client.available() && (timeout-- > 0)){
      delay(100);
    }
    if(!!!client.available()) {
       Serial.println("No response, going back to sleep");
    }
    while(client.available()){
      Serial.write(client.read());
    }
    
    Serial.println("\nclosing connection");
    client.stop();
  }
}





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
#include "sensor.h"

// Globals
const char* ver = "1.0.0";

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
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  // Initialise the LED
  if (!initLED()) {
    Serial.println("Failed to initialise the LED");
    return;
  }

  // Initialise the WiFi
  if (!initWifi()) {
    Serial.println("Failed to initialise the WiFi");
    return;
  }

  // Initialise the Web Server
  if (!initWebServer()) {
    Serial.println("Failed to initialise the Web Server");
    return;
  }

  // Initialise the Sensor
  if (!initSensor()) {
    Serial.println("Failed to initialise the PIR Sensor");
    return;
  }
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
  //makeIFTTTRequest();
  //alarm_triggered = false;
  //delay(10000);
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





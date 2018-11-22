#include <ESP8266WiFi.h>
#include "config.h"

// Pins
const int PIN_SENSOR = D5; // The pin the sensor is connected to

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
  Serial.begin(115200); 
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR), onSensorTripped, RISING);
}



/**
 * Application Loop
 */
void loop() {
  if (alarm_triggered) {
    sendTriggerToIFTTT();
  }
  delay(100);
}



/**
 * 
 */
void sendTriggerToIFTTT() {
  Serial.println("Sensor Tripped - Sending Trigger to IFTTT");
  initWifi();
  makeIFTTTRequest();
  alarm_triggered = false;
  delay(10000);
}



// Establish a Wi-Fi connection with your router
void initWifi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to: "); 
    Serial.print(ssid);
    WiFi.begin(ssid, password);  
  
    int timeout = 10 * 4; // 10 seconds
    while(WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
      delay(250);
      Serial.print(".");
    }
    Serial.println("");
  
    if(WiFi.status() != WL_CONNECTED) {
       Serial.println("Failed to connect!");
    }
  
    Serial.print("WiFi connected."); 
    Serial.print("IP address: "); 
    Serial.println(WiFi.localIP());
  }
}



// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest() {
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

#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>

boolean wifiConnected = false;
boolean accessPointMode = false;

/**
 * Connect to the WiFi network (if not already connected)
 * @returns boolean true if wifi could be connected
 */
boolean initWifi() {
  if (WiFi.status() != WL_CONNECTED) {

    // Don't bother if we don't have a configuration yet
    if (ssid != "") {
      //TODO: allow multiple wifi endpoints using wifiMulti (https://tttapa.github.io/ESP8266/Chap11%20-%20SPIFFS.html)
      //wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1");   // add Wi-Fi networks you want to connect to
      //wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
      //wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");
        
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
  
      wifiConnected = (WiFi.status() == WL_CONNECTED);
    }
  }

  return wifiConnected;
}



/**
 * Broadcast a WiFi network for the purposes of initialising the Wifi
 * @returns boolean true if a the access point was initialised
 */
boolean initAccessPointMode() {
  accessPointMode = true;

  Serial.print("Configuring Open WiFi Access Point: ");
  Serial.println(apssid);
  
  WiFi.mode(WIFI_AP_STA);
  //  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00  
  WiFi.softAP(apssid); // No password makes it an open network

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  wifiConnected = true;
  return wifiConnected;
}

#endif

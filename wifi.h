#ifndef WIFI_H
#define WIFI_H

boolean wifiConnected = false;

/**
 * Connect to the WiFi network (if not already connected)
 * @returns boolean true if wifi could be connected
 */
boolean initWifi() {
  if (WiFi.status() != WL_CONNECTED) {
    
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

  return wifiConnected;
}

#endif

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESP8266WebServer.h>
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "config.h"
#include "ledcontrol.h"
#include "FS.h"

AsyncWebServer server(serverPort);


/**
 * TODO: Complete this function
 * Handles requests that cannot be bound to endpoints
 */
void handleNotFound(){
//  Serial.print("WebServer: Client accessed unknown path '");
//  Serial.print(server.uri());
//  Serial.println("'");
//
//  String message = "File Not Found\n\n";
//  message += "URI: ";
//  message += server.uri();
//  message += "\nMethod: ";
//  message += (server.method() == HTTP_GET)?"GET":"POST";
//  message += "\nArguments: ";
//  message += server.args();
//  message += "\n";
//  for (uint8_t i=0; i<server.args(); i++){
//    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
//  }
//  server.send(404, "text/plain", message);
}


/**
 * TODO: Complete this
 * Specifically handle requests to change the RGB values of the nightlight
 */
void handleRGB() {
//  byte r = 0;
//  byte g = 0;
//  byte b = 0;
//  
//  for (int i = 0; i < server.args(); i++) {
//    if (server.argName(i) == "r") {
//      r = server.arg(i).toInt();
//    }
//    
//    if (server.argName(i) == "g") {
//      g = server.arg(i).toInt();
//    }
//
//    if (server.argName(i) == "b") {
//      b = server.arg(i).toInt();
//    }
//  } 
//
//  // Debug
//  Serial.print("WebServer: RGB Change Request to RGB(");
//  Serial.print(r);
//  Serial.print(", ");
//  Serial.print(g);
//  Serial.print(", ");
//  Serial.print(b);
//  Serial.println(")");
//
//  // Request the change to the LED
//  setLEDColor(r, g, b);
//  
//  server.send(200, "text/plain", "OK");       //Response to the HTTP 
}


/**
 * Used when serving HTML files to replace key variables in the HTML with
 * current state data.
 */
String templateProcessor(const String& var){
  Serial.println(var);
//  if(var == "STATE"){
//    if(digitalRead(ledPin)){
//      ledState = "ON";
//    }
//    else{
//      ledState = "OFF";
//    }
//    Serial.print(ledState);
//    return ledState;
//  }
  return String();
}


/**
 * Determine the Mime Type of a file based on its extension
 * @param String& filename the name of the file to check
 * @return char* the calculated mime-type of the file in question
 */
String getMimeType(const String& fileName){
  // TODO: turn this into a mapped array at some point
  if (fileName.endsWith(".html")) {
    return "text/html";
  } else if (fileName.endsWith(".css")) {
    return "text/css";
  } else if (fileName.endsWith(".js")) {
    return "text/javascript";
  }
  return "text/plain";
}


/**
 * Initialise the Web Server
 * @returns boolean true if the Web Server was started
 */
boolean initWebServer() {

  // There are different routes depending on the mode
  if (accessPointMode) {
    // Route for root / web page (apmode.html)
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/apmode.html", String(), false, templateProcessor);
    });
  } else {
    // Route for root / web page (index.html)
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", String(), false, templateProcessor);
    });
  }

  // Restart the device
  server.on("/restart", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Cool. Restarting...");
    delay(100);
    resetDevice();
  });

  // Forget the current wifi configuration and restart the device
  server.on("/forgetwifi", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Cool. I'll forget the curret wifi settings. Rebooting...");
    delay(100);
    const char* newSSID = "";
    const char* newPassword = "";
    setWiFiSettings(newSSID, newPassword);
  });

  // Set the wifi access point details
  server.on("/setwifi", HTTP_POST, [](AsyncWebServerRequest *request){
    const char* newSSID = request->arg("wifiSSID").c_str();
    const char* newPassword = request->arg("wifiPassword").c_str();

    request->send(200, "text/plain", "Cool. Updating the WiFi config and Re-booting to check the settings out...");
    delay(100);

    setWiFiSettings(newSSID, newPassword);
  });

  //server.on("/rgb", HTTP_GET, handleRGB);

  // All other Files / Routes
  server.onNotFound([](AsyncWebServerRequest *request){
    // Attempt to load the file from the SPIFFS file system
    if ((request->method() == HTTP_GET) && SPIFFS.exists(request->url())) {
      request->send(SPIFFS, request->url(), getMimeType(request->url()));
    } 

    // Return a 404
    else {
      Serial.print("Unknown Resource: ");
      Serial.print(request->url());
      Serial.print(" method: ");
      Serial.println(request->methodToString());
      request->send(404, "text/plain", "The content you are looking for was not found.");
    }
  });
 
  server.begin();
  Serial.print("HTTP server started on port ");
  Serial.println(serverPort);
}

#endif

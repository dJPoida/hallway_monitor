#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESP8266WebServer.h>
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "config.h"
#include "ledcontrol.h"
#include "sensor.h"
#include "FS.h"

AsyncWebServer server(serverPort);



/**
 * Handles setting the nightlight on or off
 */
void handleSetNightlightOn(AsyncWebServerRequest *request){
  const boolean newOn = request->getParam("on")->value() == "true";
  setNightlightOn(newOn);
  
  request->send(200, "text/json", "{\"success\":true}");
}



/**
 * Handles enabling or disabling the motion sensor (arming the alarm)
 */
void handleSetSensorEnabled(AsyncWebServerRequest *request){
  const boolean newSensorEnabled = request->getParam("sensorEnabled")->value() == "true";
  setSensorEnabled(newSensorEnabled);
  
  request->send(200, "text/json", "{\"success\":true}");
}




/**
 * Specifically handle requests to change the RGB values of the nightlight
 */
void handleSetNightlightRGB(AsyncWebServerRequest *request) {
  byte r = request->getParam("r")->value().toInt();
  byte g = request->getParam("g")->value().toInt();
  byte b = request->getParam("b")->value().toInt();
  
  // Debug
//  Serial.print("WebServer: RGB Change Request to RGB(");
//  Serial.print(r);
//  Serial.print(", ");
//  Serial.print(g);
//  Serial.print(", ");
//  Serial.print(b);
//  Serial.println(")");

  // Request the change to the LED
  setLEDColor(r, g, b);

  request->send(200, "text/json", "{\"success\":true}");
}



/**
 * Handles restarting the device
 */
void handleRestart(AsyncWebServerRequest *request){
    flagReset();
    request->send(200, "text/json", "{\"success\":true}");
}



/**
 * Handles forgetting the current WiFi details
 */
void handleForgetWiFi(AsyncWebServerRequest *request){
    setWiFiSettings("", "");
    
    request->send(200, "text/json", "{\"success\":true}");
}



/**
 * Handles setting new WiFi connection details
 */
void handleSetWiFi(AsyncWebServerRequest *request){
    AsyncWebParameter* newSSID = request->getParam("wifiSSID");
    AsyncWebParameter* newPassword = request->getParam("wifiPassword");
    
    setWiFiSettings(newSSID->value(), newPassword->value());
    
    request->send(200, "text/json", "{\"success\":true}");
}



/**
 * Handles setting new IFTTT Configuration Details
 */
void handleSetIFTTTDetails(AsyncWebServerRequest *request){
    AsyncWebParameter* newIFTTTEnabled = request->getParam("iftttEnabled");
    AsyncWebParameter* newIFTTTServer = request->getParam("iftttServer");
    AsyncWebParameter* newIFTTTEndpoint = request->getParam("iftttEndpoint");
    
    setIFTTTSettings(newIFTTTEnabled->value() == "true", newIFTTTServer->value(), newIFTTTEndpoint->value());
    
    request->send(200, "text/json", "{\"success\":true}");
}



/**
 * Handles returning the current config as a JSON response for use in the UI
 * Best part about this is that we can just read the config file from the SPIFFS
 * filesystem rather than building some string in the config unit because we
 * update and write the config to that JSON on each change anyhow.
 */
void handleGetConfig(AsyncWebServerRequest *request){
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    request->send(500, "text/json", "{\"success\":false; \"error\":\"Failed to open config file\"}");
  } else {
    // Build the struct with the "success" flag for the UI to know the AJAX call succeeded
    String response = "{\"success\":true, \"config\":";
    while (configFile.available()){
        response += char(configFile.read());
    }
    response += "}";

    // Close the file
    configFile.close();

    // Send the response
    request->send(200, "text/json", response);
  }    
}



/**
 * Handles responding to an attempt to locate the device
 * After setting the WiFi hotspot details, there's no way of knowing what IP address
 * This device received. So the AP mode website will attempt to load this endpoint on
 * every single IP address in the subnet, looking for a response.
 * When it finds this device and hits this endpoint, it will respond to the 
 * "marco" request with a "polo" response. The AP mode website will then re-direct the
 * browser to the device control page at the located IP.
 */
void handleLocateDevice(AsyncWebServerRequest *request){
  String response = "";
  response += "{\"success\":true, \"echo\":\"polo\", \"ipAddress\":\"";
  response += localIPAddress;
  response += "\"}";
  request->send(200, "text/json", response);
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
  } else if (fileName.endsWith(".svg")) {
    return "image/svg+xml";
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
  server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request){
    handleRestart(request);
  });

  // Forget the current wifi configuration and restart the device
  server.on("/forgetwifi", HTTP_GET, [](AsyncWebServerRequest *request){
    handleForgetWiFi(request);
  });

  // Set the wifi access point details
  server.on("/setwifi", HTTP_GET, [](AsyncWebServerRequest *request){
    handleSetWiFi(request);
  });

  // Set the wifi access point details
  server.on("/setnightlighton", HTTP_GET, [](AsyncWebServerRequest *request){
    handleSetNightlightOn(request);
  });
  
  // Set the wifi access point details
  server.on("/setnightlightrgb", HTTP_GET, [](AsyncWebServerRequest *request){
    handleSetNightlightRGB(request);
  });

  // Set the enabled state of the sensor
  server.on("/setsensorenabled", HTTP_GET, [](AsyncWebServerRequest *request){
    handleSetSensorEnabled(request);
  });
  
  // Get the current config as a JSON object
  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){
    handleGetConfig(request);
  });

  // Respond to a request to locate the device
  server.on("/marco", HTTP_GET, [](AsyncWebServerRequest *request){
    handleLocateDevice(request);
  });

  // All other Files / Routes
  server.onNotFound([](AsyncWebServerRequest *request){
    // Attempt to load the file from the SPIFFS file system
    if ((request->method() == HTTP_GET) && SPIFFS.exists(request->url())) {
      request->send(SPIFFS, request->url(), getMimeType(request->url()));
    } 

    // Handle HTTP_OPTIONS
    else if (request->method() == HTTP_OPTIONS) {
      request->send(200);
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

  // Some default headers for handling CORS problems in javascript
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  
  server.begin();
  Serial.print("HTTP server started on port ");
  Serial.println(serverPort);
}

#endif

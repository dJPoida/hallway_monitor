#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "config.h"
#include "ledcontrol.h"
#include "FS.h"

ESP8266WebServer server(serverPort);



/**
 * Handles requests that cannot be bound to endpoints
 */
void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}


/**
 * Specifically handle requests to change the RGB values of the nightlight
 */
void handleRGB() {
  byte r = 0;
  byte g = 0;
  byte b = 0;
  
  for (int i = 0; i < server.args(); i++) {
    if (server.argName(i) == "r") {
      r = server.arg(i).toInt();
    }
    
    if (server.argName(i) == "g") {
      g = server.arg(i).toInt();
    }

    if (server.argName(i) == "b") {
      b = server.arg(i).toInt();
    }
  } 

  setLEDColor(r, g, b);
  
  server.send(200, "text/plain", "OK");       //Response to the HTTP 
}



/**
 * Initialise the Web Server
 * @returns boolean true if the Web Server was started
 */
boolean initWebServer() {
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/rgb", handleRGB);
  server.serveStatic("/", SPIFFS, "/client/");
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.print("HTTP server started on port ");
  Serial.println(serverPort);
}

#endif

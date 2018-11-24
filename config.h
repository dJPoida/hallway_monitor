#ifndef CONFIG_H
#define CONFIG_H

// WiFi Settings
// TODO: setup multiple WiFi SSIDs and Passwords
const char* ssid     = "ELDROIDZ";
const char* password = "pkmj4eva";
const int serverPort = 60691;

// IFTT Notification Endpoint
const boolean enableIftttNotification = false;
const char* iftttServer = "maker.ifttt.com";
const char* iftttEndpoint = "/trigger/maddie_spotted/with/key/c3nx5tMYazgNfI7rHTRS4y";

// Defaults
const boolean defaultLED_R = 255;
const boolean defaultLED_G = 255;
const boolean defaultLED_B = 255;

#endif

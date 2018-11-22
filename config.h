#ifndef CONFIG_H
#define CONFIG_H

// WiFi Settings
// TODO: setup multiple WiFi SSIDs and Passwords
const char* ssid     = "ELDROIDZ";
const char* password = "pkmj4eva";

// IFTT Notification Endpoint
const boolean enableIftttNotification = false;
const char* iftttServer = "maker.ifttt.com";
const char* iftttEndpoint = "/trigger/maddie_spotted/with/key/c3nx5tMYazgNfI7rHTRS4y";

#endif

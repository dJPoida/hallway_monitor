# Hallway Monitor (aka Cheeky Monkey Detector)
Arduino / ESP8266 Motion Sensor & Night Light\
**Author:** Peter Eldred\
**Date:** 2018-11

## Background
Our little 2.5year old is getting cheeky and loves to sneak out of bed while Mum and Dad are watching TV that is entirely inappropriate (think Game of Thrones, the red wedding!).

This project started with the idea that I could utilise the power socket on the wall just outside her room to rig up a WiFi enabled sensor which would trigger either an alert or automatically pause our TV when she sneaks out.

I needed to be able to arm / disarm the sensor (we don't want it triggering all day every day) so It needed to have a web interface or some kind of hosted configuration page, and I wanted to be able to hook it into Google Home so I didn't have to use my phone to arm / disarm it.

After that, the wife asked if it could have a night light to make trekking down the hall to deal with kids easier at night, so I figured - let's put an RGB led in there and make the colour customisable too!

## Hardware
- WeMos D1.R2 Arduino Development Board
- HC-SR501 PIR Motion Sensor
- RGB LED + Appropriate resistors
- A Crappy 5v USB Wall charger I had lying around

## Dependencies
- This project depends on the [ESP8266 Sketch Data Upload Tool](http://esp8266.github.io/Arduino/versions/2.0.0/doc/filesystem.html) for storing the website data on the flash memory of the ESP8266.
- The web server component of this project utilises the [ESP Async TCP](https://github.com/me-no-dev/ESPAsyncTCP) and [ESP Async Web Server](https://github.com/me-no-dev/ESPAsyncWebServer) libraries for asynchronously delivering the control interfaces.

## Compilation and Flashing the device
TODO

## Operation of the device
When first turned on, the device will flash red, indicating it is not configured to connect to the local WiFi.

At this point, using a mobile phone, scan for WiFi networks and an open access point called `Hallway Monitor` will be available.

Connect to the Access Point and navigate your browser to [http://192.168.4.1](http://192.168.4.1). You will be presented with a configuration page where you can input a WiFi SSID and Password you want the device to connect to.

Submitting the configuration will re-boot the device. It will attempt to connect to the desired WiFi hotspot, and if successful, the page you were viewing will navigate to [http://hallwaymonitor](http://hallwaymonitor) and you should see the Hallway Monitor control panel.

### Troubleshooting
If you can't navigate to [http://hallwaymonitor](http://hallwaymonitor) then it is likely your network doesn't support local hostname resolution. In this case, your only opportunity is to connect to your router and look for the "Hallway Monitor" attached device, grab it's IP address and plug it straight into your browser.

## API
| Route | Method | Parameters | comments |
|:--- |:--- |:--- |:--- |
| /restart | POST | | Restart the device |
| /forgetwifi | POST | | Clear the WiFi access point settings and restart the device |
| /setwifi | POST | (string) wifiSSID<br/>(string) wifiPassword | Update the access point and restarts the device |

## TODO
- Implement Multiple WiFi access point configuration
- Write last state to SPIFFS or FLASH and re-load on start-up
- Build the Configuration Website (Pseudo App)
- Connect to Website using Web Sockets
    - Accept state changes via web sockets
    - Implement push to clients when state changes
- Create endpoints for all configuration / settings
- Get MDNS working (responding on http://hallway-monitor.local)
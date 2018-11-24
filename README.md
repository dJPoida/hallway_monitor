# Hallway Monitor
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

## TODO
- Implement Multiple WiFi access point configuration
- Write last state to SPIFFS or FLASH and re-load on start-up
- Build the Configuration Website (Pseudo App)
- Connect to Website using Web Sockets
    - Accept state changes via web sockets
    - Implement push to clients when state changes
- Create endpoints for all configuration / settings
- Connect the sensor
- Hotspot mode when WiFi connection fails
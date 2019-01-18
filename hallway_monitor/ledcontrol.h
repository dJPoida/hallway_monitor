#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "pins.h"
#include "config.h"

// Globals
bool flashStateOn = false;     
unsigned long previousFlashMillis = 0;
const long flashInterval = 1000;


/**
 * Set the LED to a specific RGB color
 * 
 * @param byte r the red value (0-255)
 * @param byte g the green value (0-255)
 * @param byte b the blue value (0-255) 
 */
void setLEDColor(byte r, byte g, byte b) {

  int offsetR = (1023 - map(r, 0, 255, 0, (int)(1023 * redBias)));
  int offsetG = (1023 - map(g, 0, 255, 0, (int)(1023 * greenBias)));
  int offsetB = (1023 - map(b, 0, 255, 0, (int)(1023 * blueBias)));

  // Write the Analog Values to the Pins
  analogWrite(PIN_LED_RED, offsetR);
  analogWrite(PIN_LED_GREEN, offsetG);
  analogWrite(PIN_LED_BLUE, offsetB);
  
  // Ensure the LEDs are powered
  digitalWrite(PIN_LED_POWER, HIGH);
}


/**
 * Flash the LEDs when in Access Point Mode as the device needs configuring to the WiFi
 */
void handleFlashAPModeLED() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousFlashMillis >= flashInterval) {
    previousFlashMillis = currentMillis;   
    if (flashStateOn == false) {
      flashStateOn = true;
      setLEDColor(255, 0, 0);
    } else {
      flashStateOn = false;
      digitalWrite(PIN_LED_POWER, LOW);
    }
  }
}



/**
 * Set the nightlight on or off
 * @var boolean newOn whether the nightlight should be on or off
 */
void setNightlightOn(boolean newOn) {
    Serial.print("Setting the nightlight to: '");
    if (newOn) {
      Serial.println("ON");
    } else {
      Serial.println("OFF");
    }

    nightlightOn = newOn;
    digitalWrite(PIN_LED_POWER, nightlightOn);

    // Save the updated config.
    saveConfig();
}



/**
 * Initialise the LED
 * @returns boolean true if the sensor was initialised
 */
boolean initLED() {
  pinMode(PIN_LED_POWER, OUTPUT);
  digitalWrite(PIN_LED_POWER, nightlightOn);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);

  // TODO: somekind of check to see if the LED was actually initialised
  return true;
}

#endif

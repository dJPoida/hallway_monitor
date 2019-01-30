#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "pins.h"
#include "config.h"

// Globals
bool flashStateOn = false;     
unsigned long previousFlashMillis = 0;
const long flashInterval = 1000;


/**
/ Write the current LED state to the LED Pins
*/
void updateLED() {
  int offsetR = (1023 - map(LED_R, 0, 255, 0, (int)(1023 * redBias)));
  int offsetG = (1023 - map(LED_G, 0, 255, 0, (int)(1023 * greenBias)));
  int offsetB = (1023 - map(LED_B, 0, 255, 0, (int)(1023 * blueBias)));

  // Write the Analog Values to the Pins
  analogWrite(PIN_LED_RED, offsetR);
  analogWrite(PIN_LED_GREEN, offsetG);
  analogWrite(PIN_LED_BLUE, offsetB);
  
  // Ensure the LEDs are powered (if required)
  digitalWrite(PIN_LED_POWER, accessPointMode ? flashStateOn : nightlightOn);  
}



/**
 * Set the LED to a specific RGB color
 * 
 * @param byte r the red value (0-255)
 * @param byte g the green value (0-255)
 * @param byte b the blue value (0-255) 
 */
void setLEDColor(byte r, byte g, byte b) {
  LED_R = r;
  LED_G = g;
  LED_B = b;

  updateLED();
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
      LED_R = 255;
      LED_G = 0;
      LED_B = 0;
    } else {
      flashStateOn = false;
    }
    updateLED();
  }
}



/**
 * Set the nightlight on or off
 * @var boolean newOn whether the nightlight should be on or off
 */
void setNightlightOn(boolean newOn) {
    Serial.print("Setting the nightlight to: '");
    if (newOn) {
      Serial.println("ON'");
    } else {
      Serial.println("OFF'");
    }

    nightlightOn = newOn;

    updateLED();

    // Save the updated config.
    saveConfig();
}



/**
 * Initialise the LED
 * @returns boolean true if the sensor was initialised
 */
boolean initLED() {
  pinMode(PIN_LED_POWER, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);

  updateLED();
  
  return true;
}

#endif

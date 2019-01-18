#ifndef RESET_H
#define RESET_H

#include "pins.h"

/**
 * Resets the device by pulling the reset pin high
 */
void resetDevice() {
  Serial.println("Restarting device...");
  digitalWrite(PIN_RESET, LOW);
  delay(5000);
}



/**
 * Initialise any pins not initialised by their own dedicated controllers
 * @returns boolean whether the init was successful
 */
bool initReset() {
    pinMode(PIN_RESET, OUTPUT);
    digitalWrite(PIN_RESET, HIGH);
    return true;
}

#endif


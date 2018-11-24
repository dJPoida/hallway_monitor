#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "pins.h"

void setLEDColor(byte r, byte g, byte b) {
  analogWrite(PIN_LED_RED, 1023 - map(r, 0, 255, 0, 1023));
  analogWrite(PIN_LED_GREEN, 1023 - map(g, 0, 255, 0, 1023));
  analogWrite(PIN_LED_BLUE, 1023 - map(b, 0, 255, 0, 1023));
  digitalWrite(PIN_LED_POWER, HIGH);
  Serial.print("Setting LED to RGB(");
  Serial.print(r);
  Serial.print(", ");
  Serial.print(g);
  Serial.print(", ");
  Serial.print(b);
  Serial.println(")");
}

void initLED() {
  pinMode(PIN_LED_POWER, OUTPUT);
  digitalWrite(PIN_LED_POWER, LOW);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);
}

#endif

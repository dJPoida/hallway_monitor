#ifndef SENSOR_H
#define SENSOR_H

#include "config.h"
#include "pins.h"

// States
bool alarm_triggered = false;

/**
 * Fired when the alarm interrupt is triggered
 */
void onSensorTripped()
{
  alarm_triggered = !alarm_triggered;
}

/**
 * Initialise the Sensor
 * @returns boolean true if the sensor was initialised
 */
boolean initSensor() {
  pinMode(PIN_SENSOR, INPUT);

  // Connect the Sensor Interrupt
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR), onSensorTripped, CHANGE);

  // TODO: some kind of check in the future?
  return true;
}

#endif

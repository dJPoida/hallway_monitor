#ifndef SENSOR_H
#define SENSOR_H

#include "config.h"
#include "pins.h"

// States
bool alarmTriggered = false;


/**
 * Fired when the alarm interrupt is triggered
 */
void onSensorTripped()
{
  if (sensorEnabled) {
    alarmTriggered = !alarmTriggered;
  }
}



/**
 * Initialise the Sensor
 * @returns boolean true if the sensor was initialised
 */
boolean initSensor() {
  pinMode(PIN_SENSOR, INPUT);

  // Connect the Sensor Interrupt
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR), onSensorTripped, CHANGE);

  return true;
}

#endif

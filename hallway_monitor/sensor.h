#ifndef SENSOR_H
#define SENSOR_H

#include "config.h"
#include "pins.h"
#include "ifttt.h"

// States
bool alarmTriggered = false;
bool alarmReported = false;


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



/**
 * Enable or disable the sensor (arm the alarm)
 * @var boolean newSensorEnabled whether the sensor should be enabled or not
 */
void setSensorEnabled(boolean newSensorEnabled) {
  if (newSensorEnabled) {
    Serial.println("Arming the Motion Sensor");
  } else {
    Serial.println("Disarming the Motion Sensor");
  }
  
  sensorEnabled = newSensorEnabled;

  // Reset the alarmReported flag so we listen for future events
  if (sensorEnabled) {
    alarmReported = false;
  }
  
  // Save the updated config.
  lazyWriteConfig();
}



/**
 * When the sensor is tripped - the alarm needs to report the activity to the endpoint
 * Having done so - it latches the alarmReported flag.
 * This basically resets that flag.
 */
void resetSensor() {
  alarmReported = false;
}



/**
 * called by the main loop of the program to monitor whether the alarm has been triggered.
 */
void checkSensor() {
    // Check to see if the alarm has been triggered
    if (alarmTriggered && !alarmReported) {
      alarmReported = true;
      Serial.println("Sensor Tripped!");

      if (iftttEnabled) {
        sendTriggerToIFTTT();
      }
    }
}

#endif

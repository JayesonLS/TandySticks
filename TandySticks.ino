/* 
  TandySticks
  Adaptor that converts vintage Tandy Joysticks to USB HID devices.

  This file is hereby placed in the public domain.
*/

#include "TandyStick.h"

static const unsigned long UPDATE_PERIOD = 8333;            // 1/120th of a second.
static const int           UPDATES_BEFORE_DISCONNECT = 12;  // 1/10th second delay after last detection.
static const int           UPDATES_TO_AVERAGE = 3;          // 1/60th of a second.
static const int           UPDATES_FOR_DEBOUNCE = 6;        // 1/20th of a second - max press rate is 10 per second.

TandyStick stick0(Joystick + 0, A3, A2, A6, 2, 3);
TandyStick stick1(Joystick + 1, A1, A0, A7, 5, 7);

unsigned long sNextUpdateEnd;

void setup() 
{
  // Enable for logging.
  // Serial.begin(38400);

  TandyStick::sUpdatesBeforeDisconnect = UPDATES_BEFORE_DISCONNECT;
  TandyStick::sUpdatesToAverage = UPDATES_TO_AVERAGE;
  TandyStick::sUpdatesForDebounce = UPDATES_FOR_DEBOUNCE;

  stick0.Setup();
  stick1.Setup();

  sNextUpdateEnd = micros() + UPDATE_PERIOD;
}

void loop() {

  stick0.BeginUpdate();
  stick1.BeginUpdate();

  do
  {
    stick0.TickUpdate();
    stick1.TickUpdate();
  } while ((signed long)(sNextUpdateEnd - micros()) > 0);
    

  stick0.EndUpdate();
  stick1.EndUpdate();

  signed long updateDelta = (signed long)(micros() - sNextUpdateEnd);
  if (updateDelta < (UPDATE_PERIOD / 2))
  {
    sNextUpdateEnd += UPDATE_PERIOD;
  }
  else
  {
    sNextUpdateEnd = micros() + UPDATE_PERIOD;
  }
}


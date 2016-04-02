/* 
  TandySticks
  Adaptor that converts vintage Tandy Joysticks to USB HID devices.

  This file is hereby placed in the public domain.
*/

#include "TandyStick.h"

TandyStick stick0(Joystick + 0, A3, A2, A6, 2, 3);
TandyStick stick1(Joystick + 1, A1, A0, A7, 5, 7);

void setup() 
{
#if LOGGING
  Serial.begin(38400);
#endif // LOGGING

  stick0.Setup();
  stick1.Setup();
}

void loop() {

  delay(33);

  stick0.BeginUpdate();
  stick1.BeginUpdate();

  stick0.TickUpdate();
  stick1.TickUpdate();

  stick0.EndUpdate();
  stick1.EndUpdate();

#if LOGGING
  Serial.println("Heartbeat"); 
#endif // LOGGING
}


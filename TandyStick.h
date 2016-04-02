/* 
  TandyStick.h
  Class to manage a single joystick.

  This file is hereby placed in the public domain.
*/

#ifndef TANDY_STICK_H
#define TANDY_STICK_H

#define LOGGING 1

#include "Joystick2.h"

class TandyStick
{
private:
  // Joystick specific configuration.
  Joystick_ *mJoystick;
  uint8_t mXAxisAnalogPin;
  uint8_t mYAxisAnalogPin;
  uint8_t mDetectAnalogPin;
  uint8_t mButton0DigitalPin; 
  uint8_t mButton1DigitalPin;

  // Tracking of previous update(s).

  // Tracking of values during an update.
  long accumulatedXRead;
  long accumulatedYRead;
  long accumulatedDectectRead;
  bool hadPositiveDetection;
  bool hadButton0Down;
  bool hadButton0Up;
  bool hadButton1Down;
  bool hadButton1Up;

  int8_t  CalculateAxisValue(long accumulatedAxisValues, long accumulatedDetectValue);
  
public:
  TandyStick(Joystick_ *joystick, 
             uint8_t xAxisAnalogPin, 
             uint8_t yAxisAnalogPin, 
             uint8_t detectAnalogPin, 
             uint8_t button0DigitalPin, 
             uint8_t button1DigitalPin);

  void Setup();

  void BeginUpdate();
  void TickUpdate();
  void EndUpdate();
};

#endif // TANDY_STICK_H 


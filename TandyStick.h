/* 
  TandyStick.h
  Class to manage a single joystick.

  This file is hereby placed in the public domain.
*/

#ifndef TANDY_STICK_H
#define TANDY_STICK_H

#include "Joystick2.h"

static const int MAX_PREVIOUS_READS = 10;

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
  long mPreviousXReads[MAX_PREVIOUS_READS];
  long mPreviousYReads[MAX_PREVIOUS_READS];
  long mPreviousDetectReads[MAX_PREVIOUS_READS];
  int mUpdatesSinceLastDetection;
  bool mButton0Down;
  int mUpdatesSinceLastButton0Latch;
  bool mButton1Down;
  int mUpdatesSinceLastButton1Latch;

  // Tracking of values during an update.
  long mAccumulatedXRead;
  long mAccumulatedYRead;
  long mAccumulatedDetectRead;
  bool mHadPositiveDetection;
  bool mHadButton0Down;
  bool mHadButton0Up;
  bool mHadButton1Down;
  bool mHadButton1Up;

  int8_t  CalculateAxisValue(long accumulatedAxisValues, long accumulatedDetectValue);
  
public:
  static int sUpdatesBeforeDisconnect;
  static int sUpdatesToAverage;
  static int sUpdatesForDebounce;

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


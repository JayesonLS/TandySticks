/* 
  TandyStick.cpp
  Class to manage a single joystick.

  This file is hereby placed in the public domain.
*/

#include "TandyStick.h"

int TandyStick::sUpdatesBeforeDisconnect = 1;
int TandyStick::sUpdatesToAverage = 1;
int TandyStick::sUpdatesForDebounce = 1;

TandyStick::TandyStick(Joystick_ *joystick, 
                       uint8_t xAxisAnalogPin, 
                       uint8_t yAxisAnalogPin, 
                       uint8_t detectAnalogPin, 
                       uint8_t button0DigitalPin, 
                       uint8_t button1DigitalPin)
{
  mJoystick = joystick;
  mXAxisAnalogPin = xAxisAnalogPin;
  mYAxisAnalogPin = yAxisAnalogPin;
  mDetectAnalogPin = detectAnalogPin;
  mButton0DigitalPin = button0DigitalPin; 
  mButton1DigitalPin = button1DigitalPin;

  memset(mPreviousXReads, 0, sizeof(mPreviousXReads));
  memset(mPreviousYReads, 0, sizeof(mPreviousYReads));
  memset(mPreviousDetectReads, 0, sizeof(mPreviousDetectReads));
  mUpdatesSinceLastDetection = 0;
  mButton0Down = false;
  mUpdatesSinceLastButton0Latch = 0;
  mButton1Down = false;
  mUpdatesSinceLastButton1Latch = 0;

  mAccumulatedXRead = 0;
  mAccumulatedYRead = 0;
  mAccumulatedDetectRead = 0; 
  mHadPositiveDetection = false;
  mHadButton0Down = false;
  mHadButton0Up = false;
  mHadButton1Down = false;
  mHadButton1Up = false;
}

void TandyStick::Setup()
{
  mJoystick->begin(false);

  pinMode(mButton0DigitalPin, INPUT_PULLUP);
  pinMode(mButton1DigitalPin, INPUT_PULLUP);
}

void TandyStick::BeginUpdate()
{
  mAccumulatedXRead = 0;
  mAccumulatedYRead = 0;
  mAccumulatedDetectRead = 0; 
  mHadPositiveDetection = false;
  mHadButton0Down = false;
  mHadButton0Up = false;
  mHadButton1Down = false;
  mHadButton1Up = false;
}

void TandyStick::TickUpdate()
{
  int detectRead = analogRead(mDetectAnalogPin);
  mAccumulatedXRead += analogRead(mXAxisAnalogPin);
  mAccumulatedYRead += analogRead(mYAxisAnalogPin);
  mAccumulatedDetectRead += detectRead;
  if (detectRead < 1023)
  {
    mHadPositiveDetection = true;
    mUpdatesSinceLastDetection = 0;
  }

  int button0Value = digitalRead(mButton0DigitalPin);
  if (button0Value == LOW)
  {
    mHadButton0Down = true;
  }
  else
  {
    mHadButton0Up = true;
  }
  
  int button1Value = digitalRead(mButton1DigitalPin);
  if (button1Value == LOW)
  {
    mHadButton1Down = true;
  }
  else
  {
    mHadButton1Up = true;
  }
}

void TandyStick::EndUpdate()
{
  // Force positive detection until timeout has expired.
  if (!mHadPositiveDetection && mUpdatesSinceLastDetection < sUpdatesBeforeDisconnect)
  {
    mUpdatesSinceLastDetection++;
    mHadPositiveDetection = true;
  }

  int8_t x;
  int8_t y;
  bool button0Down;
  bool button1Down;
  
  if (mHadPositiveDetection)
  {
    ProcessAnalog(x, y);
    button0Down = mHadButton0Down;
    button1Down = mHadButton1Down;
  }
  else
  {
    x = 0;
    y = 0;
    button0Down = false;
    button1Down = false;
  }

  SendToJoystick(x, y, button0Down, button1Down);
}

void TandyStick::ProcessAnalog(int8_t &xOut, int8_t &yOut)
{
    // Push back all the previous frame values, averaging as we go.
    long combinedXRead = mAccumulatedXRead;
    long combinedYRead = mAccumulatedYRead;
    long combinedDetectRead = mAccumulatedDetectRead;

    for (int i = sUpdatesToAverage - 1; i >= 0; i--)
    {
      long tmp = mPreviousXReads[i];
      combinedXRead += tmp;
      mPreviousXReads[i + 1] = tmp;
      
      tmp = mPreviousYReads[i];
      combinedYRead += tmp;
      mPreviousYReads[i + 1] = tmp;

      tmp = mPreviousDetectReads[i];
      combinedDetectRead += tmp;
      mPreviousDetectReads[i + 1] = tmp;
    }
    
    mPreviousXReads[0] = mAccumulatedXRead;
    mPreviousYReads[0] = mAccumulatedYRead;
    mPreviousDetectReads[0] = mAccumulatedDetectRead;
    
    xOut = CalculateAxisValue(combinedXRead, combinedDetectRead);
    yOut = CalculateAxisValue(combinedYRead, combinedDetectRead);
}


// Values read from analog inputs are in range 0 to accumulatedDetectValue and need to be converted
// to the range -127 to 127.
// It is perhaps possible for noise to cause axis value to be greater than the detect value,
// but we have the value clamping just in case. Likewise, we avoid divide by zero, which 
// could perhaps happen if the detect is not connected. Garbage values will occur, but it is
// perhaps better than finding out what happens on divide by zero.
int8_t TandyStick::CalculateAxisValue(long accumulatedAxisValues, long accumulatedDetectValue)
{
  if (accumulatedDetectValue < 1)
  {
    accumulatedDetectValue = 1;
  }
  int value = (int)(accumulatedAxisValues * 255 / accumulatedDetectValue) - 127;
  if (value < -127)
  {
    value = -127;
  }
  else if (value > 127)
  {
    value = 127;
  }
  return (int8_t)value;
}

void TandyStick::SendToJoystick(int8_t x, int8_t y, bool button0Down, bool button1Down)
{
  mJoystick->setXAxis(x);
  mJoystick->setYAxis(y);
 
  if (button0Down)
    mJoystick->pressButton(0);
  else
    mJoystick->releaseButton(0);
    
  if (button1Down)
    mJoystick->pressButton(1);
  else
    mJoystick->releaseButton(1);

  mJoystick->sendState();

}



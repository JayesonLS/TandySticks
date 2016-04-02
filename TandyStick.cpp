/* 
  TandyStick.cpp
  Class to manage a single joystick.

  This file is hereby placed in the public domain.
*/

#include "TandyStick.h"

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
}

void TandyStick::Setup()
{
  mJoystick->begin(false);

  pinMode(mButton0DigitalPin, INPUT_PULLUP);
  pinMode(mButton1DigitalPin, INPUT_PULLUP);
}

void TandyStick::BeginUpdate()
{
  
}

void TandyStick::TickUpdate()
{
  
}

void TandyStick::EndUpdate()
{
  long detectValue = analogRead(mDetectAnalogPin);
  long xAxisValue = analogRead(mXAxisAnalogPin);
  long yAxisValue = analogRead(mYAxisAnalogPin);
  int button0Value = digitalRead(mButton0DigitalPin);
  int button1Value = digitalRead(mButton1DigitalPin);

  if (detectValue == 1023)
  {
    mJoystick->setXAxis(0);
    mJoystick->setYAxis(0);
  }
  else
  {
    mJoystick->setXAxis(CalculateAxisValue(xAxisValue, detectValue));
    mJoystick->setYAxis(CalculateAxisValue(yAxisValue, detectValue));
  }
  
  if (button0Value == LOW)
    mJoystick->pressButton(0);
  else
    mJoystick->releaseButton(0);
    
  if (button1Value == LOW)
    mJoystick->pressButton(1);
  else
    mJoystick->releaseButton(1);
 
  mJoystick->sendState();
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




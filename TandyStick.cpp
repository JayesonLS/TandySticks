/* 
  TandyStick.cpp
  Class to manage a single joystick.

  This file is hereby placed in the public domain.
*/

#include "TandyStick.h"

int TandyStick::sUpdatesBeforeDisconnect = 1;
int TandyStick::sPreviousUpdatesToAverageIn = 0;
int TandyStick::sUpdatesForDebounce = 1;
int TandyStick::sAnalogRange = 127;
int TandyStick::sAnalogBias = 0;

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

  mLastXSent = -512; // Not a value that can be generated, will force a send.
  mLastYSent = -512;
  mLastButton0DownSent = false;
  mLastButton1DownSent = false;

  mAccumulatedXRead = 0;
  mAccumulatedYRead = 0;
  mAccumulatedDetectRead = 0;
  mHadButton0Down = false;
  mHadButton0Up = false;
  mHadButton1Down = false;
  mHadButton1Up = false;
  mNumUpdateTicks = 0;
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
  mHadButton0Down = false;
  mHadButton0Up = false;
  mHadButton1Down = false;
  mHadButton1Up = false;
  mNumUpdateTicks = 0;
}

void TandyStick::TickUpdate()
{
  int detectRead = analogRead(mDetectAnalogPin);
  mAccumulatedXRead += analogRead(mXAxisAnalogPin);
  mAccumulatedYRead += analogRead(mYAxisAnalogPin);
  mAccumulatedDetectRead += detectRead;

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

  mNumUpdateTicks++;
}

void TandyStick::EndUpdate()
{
  bool hadPositiveDetection = HadPositiveDetection();
  
  if (hadPositiveDetection)
  {
    mUpdatesSinceLastDetection = 0;
  }
  else if (mUpdatesSinceLastDetection < sUpdatesBeforeDisconnect)
  {
    // Force positive detection until timeout has expired.
    mUpdatesSinceLastDetection++;
    hadPositiveDetection = true;
  }

  int16_t x;
  int16_t y;
  ProcessAnalog(hadPositiveDetection, x, y);
  
  bool button0Down = ProcessButton(hadPositiveDetection, mHadButton0Down, mHadButton0Up, mButton0Down, mUpdatesSinceLastButton0Latch);
  bool button1Down = ProcessButton(hadPositiveDetection, mHadButton1Down, mHadButton1Up, mButton1Down, mUpdatesSinceLastButton1Latch);

  SendToJoystick(x, y, button0Down, button1Down);
}

bool TandyStick::HadPositiveDetection()
{
  // If no stick is connected, the 100 ohm resistor will pull the input up to 5v (read = 1023). 
  // Threshold is an average of 1022.5. This allows for some of the reads to be below 1023.
  long threshold = (((1022L << 1) + 1) * mNumUpdateTicks) >> 1;
  return mAccumulatedDetectRead <= threshold; 
}

void TandyStick::ProcessAnalog(bool stickConnected, int16_t &xOut, int16_t &yOut)
{
  if (!stickConnected)
  {
      // Force stick to center. Previous-frame averaging will still be applied.
      xOut = yOut = 0;
      return;
  }

  // Push back all the previous frame values, averaging as we go.
  long combinedXRead = mAccumulatedXRead;
  long combinedYRead = mAccumulatedYRead;
  long combinedDetectRead = mAccumulatedDetectRead;

  for (int i = sPreviousUpdatesToAverageIn - 1; i >= 0; i--)
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
int16_t TandyStick::CalculateAxisValue(long accumulatedAxisValues, long accumulatedDetectValue)
{
  if (accumulatedDetectValue < 1)
  {
    accumulatedDetectValue = 1;
  }

  long longRange = (long)sAnalogRange;
  long biasedRange = (long)(sAnalogRange + sAnalogBias);
  long value = accumulatedAxisValues * (biasedRange + 1L) * 2L / accumulatedDetectValue - biasedRange;
  long savedValue = value;
  if (value < -longRange)
  {
    value = -longRange;
  }
  else if (value > longRange)
  {
    value = longRange;
  }

  return (int16_t)value;
}

bool TandyStick::ProcessButton(bool stickConnected, bool hadButtonDown, bool hadButtonUp, bool &buttonLatchedDown, int &updatesSinceLastButtonLatch)
{
  if (stickConnected)
  {
    if (updatesSinceLastButtonLatch < sUpdatesForDebounce)
    {
      updatesSinceLastButtonLatch++;
    }
    else
    {
      if (buttonLatchedDown && hadButtonUp)
      {
        buttonLatchedDown = false;
        updatesSinceLastButtonLatch = 0;
      }
      else if (!buttonLatchedDown && hadButtonDown)
      {
        buttonLatchedDown = true;
        updatesSinceLastButtonLatch = 0;
      }
    }

    return buttonLatchedDown;
  }
  else
  {
    // Stick disconnected, reset all values.
    buttonLatchedDown = false;
    updatesSinceLastButtonLatch = 0;
    return false; 
  }
}

void TandyStick::SendToJoystick(int16_t x, int16_t y, bool button0Down, bool button1Down)
{
  if (x != mLastXSent || 
      y != mLastYSent || 
      button0Down != mLastButton0DownSent ||
      button1Down != mLastButton1DownSent)
  {     
    mLastXSent = x;
    mLastYSent = y;
    mLastButton0DownSent = button0Down;
    mLastButton1DownSent = button1Down;
  
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
}



/* 
  TandySticks
  Adaptor that converts vintage Tandy Joysticks to USB HID devices.

  Intended for Arduino ATMEGA32U devices: Pro Micro, Micro, Leonardo.
  See Readme.md for full details.

  This file is hereby placed in the public domain.
*/

#include "TandyStick.h"

static const unsigned long UPDATE_PERIOD = 8333;                // 1/120th of a second.
static const int           UPDATES_BEFORE_DISCONNECT = 12;      // 1/10th second delay after last detection.
static const int           PREVIOUS_UPDATES_TO_AVERAGE_IN = 2;  // 3/120th of a second.
static const int           UPDATES_FOR_DEBOUNCE = 6;            // 1/20th of a second - max press rate is 10 per second.

TandyStick stick0(Joystick + 0, A3, A2, A6, 2, 3);
TandyStick stick1(Joystick + 1, A1, A0, A7, 5, 7);

unsigned long sNextUpdateEnd;

void setup() 
{
  // Enable for logging.
  Serial.begin(38400);

  TandyStick::sUpdatesBeforeDisconnect = UPDATES_BEFORE_DISCONNECT;
  TandyStick::sPreviousUpdatesToAverageIn = PREVIOUS_UPDATES_TO_AVERAGE_IN;
  TandyStick::sUpdatesForDebounce = UPDATES_FOR_DEBOUNCE;
  TandyStick::sAnalogRange = 511;

  // Bias results vary between Coco versions. Three Coco examples were tested with the following results:
  //
  //                    Supply   0 level   31/32 level   63 level   0 norm   31/32 norm   63 norm
  // Coco 1 (E Board)   4.94v      0.30v      2.48v        4.70v     0.06       0.5         0.95
  // Coco 2 (Pre-TI)    5.05v      0.47v      2.55v        4.73v     0.09       0.5         0.94
  // Coco 3 (PAL)       4.95v      0.48v      2.48v        4.56v     0.10       0.5         0.92
  //
  // There is some non-linearity in the results - the 0 level bias is typically higher than the 63
  // level bias, even though the 31/32 center point is perfectly centered. For simplicity, we will
  // use the same bias for both high and low levels. 
  // 
  // There is also considerable variation in the amount of bias across the Cocos. Since all joysticks 
  // tested give full range on all Cocos, we will use something the same and middle of the road for both high and low.
  //
  // Note that this biasing assumes the microcontroller is linear all the way from 0v to the supply 
  // voltage. This appears to be correct, or very close to correct, for the ATMEGA32U4.

  // 16%. Note that the math is messy, since: Bias / (Bias + Range) = 0.16
  long percentage = 16;
  TandyStick::sAnalogBias = (int)( (TandyStick::sAnalogRange * percentage) / (100L - percentage) );

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


/*
  Joystick2.h

  Copyright (c) 2015, Matthew Heironimus
  Copyright (c) 2016, Titanium Studios Pty. Ltd.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef JOYSTICK_h
#define JOYSTICK_h

#include "HID.h"

#if !defined(_USING_HID)

#warning "Using legacy HID core (non pluggable). Make sure the selected board is Leonardo or Micro and Arduino version is >= 1.6.6."

#else // defined(_USING_HID)

//================================================================================
//================================================================================
//  Joystick (Gamepad)

class Joystick_
{
private:
	HID_    *hid; 
	bool     autoSendState;
	int16_t	 xAxis;
	int16_t	 yAxis;
	uint8_t  buttons;

public:
	Joystick_();

	void begin(bool initAutoSendState);
	void end();

	void setXAxis(int16_t value);
	void setYAxis(int16_t value);

	void setButton(uint8_t button, uint8_t value);
	void pressButton(uint8_t button);
	void releaseButton(uint8_t button);

	void sendState();
};
extern Joystick_ Joystick[2];

#endif // defined(_USING_HID)
#endif // JOYSTICK_h

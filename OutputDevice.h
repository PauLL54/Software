//  Copyright © 2019 Paul Langemeijer. All rights reserved.
#pragma once

class OutputDevice {
public:
	OutputDevice(int ledPinOn, int ledPinOff, int resetPin);

	void blinkLedsOnOff(int seconds);
	void setOn();
	void setOff();

private:
	void showLedOnState();
	void showLedOffState();

private:
	int m_ledPinOn;
	int m_ledPinOff;
	int m_resetPin;
};

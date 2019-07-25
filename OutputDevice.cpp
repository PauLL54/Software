//  Copyright © 2019 Paul Langemeijer. All rights reserved.
#include "OutputDevice.h"
#include <Arduino.h>

OutputDevice::OutputDevice(int ledPinOn, int ledPinOff, int SsrPin)
{
	m_ledPinOn    = ledPinOn;
	m_ledPinOff   = ledPinOff;
	m_SsrPin      = SsrPin;
}

void OutputDevice::setup()
{
	pinMode(m_ledPinOn,    OUTPUT);
	pinMode(m_ledPinOff,   OUTPUT);
	pinMode(m_SsrPin,      OUTPUT);
	
	setOff();
}

void OutputDevice::blinkLedsOnOff(int seconds)
{
	int count = 0;
	bool stateOn = false;
	int blinkRatePerSecond = 3;
	
	while (count < seconds * blinkRatePerSecond)
	{
		delay(1000 / blinkRatePerSecond);
		count++;
		stateOn = !stateOn;

		if (stateOn)
			showLedOnState();
		else
			showLedOffState();
	}
	
	setOff();
}

void OutputDevice::showLedOnState()
{
	digitalWrite(m_ledPinOn,  HIGH);
	digitalWrite(m_ledPinOff, LOW);
}

void OutputDevice::showLedOffState()
{
	digitalWrite(m_ledPinOn,  LOW);
	digitalWrite(m_ledPinOff, HIGH);
}

void OutputDevice::setOn()
{
	digitalWrite(m_SsrPin, HIGH);
	showLedOnState();
}

void OutputDevice::setOff()
{
	digitalWrite(m_SsrPin, LOW);
	showLedOffState();
}

//  Copyright © 2019 Paul Langemeijer. All rights reserved.
#include "AudioSignal.h"
#include <Arduino.h>

const int BufSize = 10; // hfghfgh qqqqqqqqqqqqq

AudioSignal::AudioSignal(int pinAdcPlus, int pinAdcMinus, int pinAdcRef, int treshold)
{
	m_pinAdcPlus  = pinAdcPlus;
	m_pinAdcMinus = pinAdcMinus;
	m_pinAdcRef   = pinAdcRef;
	m_treshold    = treshold;

	m_bufferIndex = 0;
	m_bufferMax   = 0;
	m_buffer      = new int[BufSize];
}

bool AudioSignal::isDetected()
{	
	int diffValue = getDiffValue();
	diffValue = abs(diffValue);
	bool audioDetected = diffValue > m_treshold;

	return audioDetected;
}

int AudioSignal::getDiffValue()
{
	int adcPlus  = analogRead(m_pinAdcPlus);
	int adcMinus = analogRead(m_pinAdcMinus);
  
  String s1 = String(adcPlus);
  String s2 = String(adcMinus);
  String s = s1 + "  " + s2;
  Serial.println(s);
  
	return  adcPlus - adcMinus;
}

int AudioSignal::getRefValue()
{
	m_buffer[m_bufferIndex] = analogRead(m_pinAdcRef);
	
	// check circular buffer
	m_bufferIndex++;
	if (m_bufferIndex < BufSize)
		m_bufferMax++;
	else
		m_bufferIndex = 0;

	long sum = 0;
	for (int i = 0; i < m_bufferMax; i++)
	{
		sum += m_buffer[i];
	}
	int average = sum / m_bufferMax;
	return average;
}	

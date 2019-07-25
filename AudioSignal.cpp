//  Copyright © 2019 Paul Langemeijer. All rights reserved.
#include "AudioSignal.h"

// When building in Arduino IDE, uncomment next line:
//#include <arduino.h>

const int BufSize = 10;

AudioSignal::AudioSignal(int pinAdc,  int pinAdcRef, int treshold)
{
	m_pinAdc    = pinAdc;
	m_pinAdcRef = pinAdcRef;
	m_treshold  = treshold;

	m_bufferIndex = 0;
	m_bufferMax   = 0;
	m_buffer      = new int[BufSize];
}

void AudioSignal::SetTreshold(int treshold)
{
	m_treshold  = treshold;
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
	int adcRef = getRefValue();
	int adc    = analogRead(m_pinAdc);
  
    String s1 = String(adc);
    String s2 = String(adcRef);
    String s = "ADC=" + s1 + " VRef=" + s2;
    //Serial.println(s);
  
	return  adcRef - adc;
}

int AudioSignal::getRefValue()
{
	m_buffer[m_bufferIndex] = analogRead(m_pinAdcRef);
  
	// check circular buffer
	m_bufferIndex++;
	if ((m_bufferMax < BufSize) && (m_bufferIndex < BufSize))
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

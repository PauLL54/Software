//  Copyright © 2019 Paul Langemeijer. All rights reserved.
#pragma once

class AudioSignal {
public:
	AudioSignal(int pinAdcPlus, int pinAdcMinus, int pinAdcRef, int treshold);

	bool isDetected();
	
private:
	int getDiffValue();
	int getRefValue();

private:
	int m_pinAdcPlus;
	int m_pinAdcMinus;
	int m_pinAdcRef;
	int m_treshold;
	
	int m_bufferIndex;
	int m_bufferMax;
	int* m_buffer;
};

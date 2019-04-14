//  Copyright © 2019 Paul Langemeijer. All rights reserved.
#pragma once

class OneshotTimer {
public:
	OneshotTimer(unsigned long duration, void (*callback)());

    void start();
    void stop();
	void setDuration(unsigned long duration);
    void update();
    
private:
    unsigned long m_duration;
    unsigned long m_stopTime;
    bool m_timerStarted;
    void (*m_callback)();
};

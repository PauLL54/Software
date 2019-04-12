//  Copyright © 2019 Paul Langemeijer. All rights reserved.
#pragma once
#include "timer.h"

class OneshotTimer {
public:
	OneshotTimer(unsigned long duration, void (*callback)(void*));

    void start();
    void stop();
	void setDuration(unsigned long duration);
    void update();
    
private:
    unsigned long m_duration;
    void (*m_callback)(void*);
    int m_id;
    Timer m_timer;
};

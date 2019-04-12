//  Copyright © 2019 Paul Langemeijer. All rights reserved.
#include "OneshotTimer.h"

OneshotTimer::OneshotTimer(unsigned long duration, void (*callback)(void*))
: m_duration(duration),
  m_callback(callback),
  m_id(TIMER_NOT_AN_EVENT)
{
}

void OneshotTimer::setDuration(unsigned long duration)
{
    m_duration = duration;
}

void OneshotTimer::start()
{
    stop();
    m_id = m_timer.after(m_duration, m_callback, (void*)0);
}

void OneshotTimer::stop()
{
    if (m_id >= 0)
    {
        m_id = m_timer.stop(m_id);
    }
}

void OneshotTimer::update()
{
    m_timer.update();
}
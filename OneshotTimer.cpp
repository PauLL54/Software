//  Copyright © 2019 Paul Langemeijer. All rights reserved.
#include "OneshotTimer.h"
#include "Arduino.h"

OneshotTimer::OneshotTimer(unsigned long duration, void (*callback)())
: m_duration(duration),
  m_stopTime(0),
  m_timerStarted(false),
  m_callback(callback)
{
}

void OneshotTimer::setDuration(unsigned long duration)
{
    if (m_timerStarted)
    {
        m_stopTime = m_stopTime - m_duration + duration;
    }
    m_duration = duration;
}

void OneshotTimer::start()
{
    m_stopTime = millis() + m_duration;
    m_timerStarted = true;
}

void OneshotTimer::stop()
{
    m_timerStarted = false;
}

void OneshotTimer::update()
{
    if (m_timerStarted && (millis() > m_stopTime))
    {
        m_timerStarted = false;
        (m_callback)();
    }
}
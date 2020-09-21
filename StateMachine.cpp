//  Copyright © 2019 Paul Langemeijer. All rights reserved.
#include "StateMachine.h"

StateMachine::StateMachine(
	OneshotTimer &musicDetectionTimer, 
	OneshotTimer &spikeDetectionTimer, 
	OneshotTimer &musicStoppingTimer,
	OutputDevice &outputDevice)
: m_musicDetectionTimer(musicDetectionTimer),
  m_spikeDetectionTimer(spikeDetectionTimer),
  m_musicStoppingTimer(musicStoppingTimer),
  m_outputDevice(outputDevice),
  m_state(State::Idle)
{
}

void StateMachine::onAudioDetected()
{
	switch (m_state)
	{
		case State::Idle:
			m_musicDetectionTimer.start();
			m_spikeDetectionTimer.start();
			setState(State::MusicStarting);
			break;

		case State::MusicStarting:
			m_spikeDetectionTimer.start();
			break;
			
		case State::MusicPlaying:	
			m_musicDetectionTimer.start();
			break;
			
		case State::MusicStopping:	
			m_musicDetectionTimer.start();
			m_spikeDetectionTimer.start();
			setState(State::MusicRestarting);
			break;
			
		case State::MusicRestarting:	
			m_spikeDetectionTimer.start();
			break;

		default:
			break;
	}
}

void StateMachine::onMusicDetectionTimerExpired()
{
	switch (m_state)
	{
		case State::MusicStarting:
			m_spikeDetectionTimer.stop();
			m_musicDetectionTimer.start();
			m_outputDevice.setOn();
			setState(State::MusicPlaying);
			break;
			
		case State::MusicPlaying:	
		    Serial.println("State::MusicPlaying");

			m_musicStoppingTimer.start();
			setState(State::MusicStopping);
			break;
			
		case State::MusicRestarting:	
			m_spikeDetectionTimer.stop();
			m_musicStoppingTimer.stop();
			m_musicDetectionTimer.start();
			setState(State::MusicPlaying);
			break;

		default:
			break;
	}
}

void StateMachine::onSpikeDetectionTimerExpired()
{
	switch (m_state)
	{
		case State::MusicStarting:
			m_musicDetectionTimer.stop();
			setState(State::Idle);
			break;
			
		case State::MusicRestarting:	
			m_musicStoppingTimer.stop();
			setState(State::Idle);
			m_outputDevice.setOff();
			break;

		default:
			break;
	}
}

void StateMachine::onMusicStoppingTimerExpired()
{
	switch (m_state)
	{
		case State::MusicStopping:
			m_spikeDetectionTimer.stop();
			m_musicDetectionTimer.stop();
			setState(State::Idle);
			m_outputDevice.setOff();
			break;
			
		case State::MusicRestarting:	
			m_spikeDetectionTimer.stop();
			m_musicDetectionTimer.stop();
			setState(State::Idle);
			m_outputDevice.setOff();
			break;

		default:
			break;
	}
}

void StateMachine::setState(State state)
{
  m_state = state;
  Serial.println(getState());
}

String StateMachine::getState()
{
	switch (m_state)
	{
		case State::Idle: 				return "Idle";
		case State::MusicStarting:		return "MusicStarting";
		case State::MusicPlaying:		return "MusicPlaying";
		case State::MusicStopping:		return "MusicStopping";
		case State::MusicRestarting:	return "MusicRestarting";
		default:
			break;
	}
	return "-";
}

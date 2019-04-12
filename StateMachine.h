//  Copyright © 2019 Paul Langemeijer. All rights reserved.
#pragma once
#include "OneshotTimer.h"
#include "OutputDevice.h"
#include <Arduino.h>

class StateMachine {
public:
	StateMachine(
		OneshotTimer &musicDetectionTimer, 
		OneshotTimer &spikeDetectionTimer, 
		OneshotTimer &musicStoppingTimer, 
		OutputDevice &outputDevice);

	void onAudioDetected();
	void onMusicDetectionTimerExpired();
	void onSpikeDetectionTimerExpired();
	void onMusicStoppingTimerExpired();

private:
enum State { 
  Idle, 
  MusicStarting, 
  MusicPlaying, 
  MusicStopping, 
  MusicRestarting };
  
  void setState(State state);
	String getState();

private:
  OneshotTimer &m_musicDetectionTimer;
	OneshotTimer &m_spikeDetectionTimer;
	OneshotTimer &m_musicStoppingTimer;
	OutputDevice &m_outputDevice;

	State m_state;
};

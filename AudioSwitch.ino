//  Copyright © 2019 Paul Langemeijer. All rights reserved.
#include "StateMachine.h"
#include "OutputDevice.h"
#include "AudioSignal.h"
#include "OneshotTimer.h"

const int LedPinOn    =  1; // PD1 pin 3
const int LedPinOff   =  2; // PD2 pin 4
const int switchOnPin =  8; // PB0 pin 14
const int TestPin     =  5; // PB5 pin 11
const int PinAdcRef   = 14; // PC0 pin 23
const int PinAdc      = 15; // PC1 pin 24
const int Treshold    =  4; // treshold value (in bits) for audio detected

const int LedBlinkTimeOnStartup = 5; // seconds

const unsigned long MusicDetectionTimeout           =    600; // ms 
const unsigned long SpikeDetectionTimeout           =    300; // ms 
const unsigned long NoMusicDetectionTimeout         = 300000; // 5 minutes
const unsigned long NoMusicDetectionTimeoutTestMode =   2000; // 2 seconds 

// Required forward declarations:
void onMusicDetectionTimerExpired(void*); 
void onSpikeDetectionTimerExpired(void*);
void onMusicStoppingTimerExpired(void*);

OneshotTimer musicDetectionTimer = OneshotTimer(MusicDetectionTimeout,   onMusicDetectionTimerExpired);
OneshotTimer spikeDetectionTimer = OneshotTimer(SpikeDetectionTimeout,   onSpikeDetectionTimerExpired);
OneshotTimer musicStoppingTimer  = OneshotTimer(NoMusicDetectionTimeout, onMusicStoppingTimerExpired);

OutputDevice outputDevice = OutputDevice(LedPinOn, LedPinOff, switchOnPin);
AudioSignal  audioSignal  = AudioSignal(PinAdc, PinAdcRef, Treshold);
StateMachine stateMachine = StateMachine(musicDetectionTimer, spikeDetectionTimer, musicStoppingTimer, outputDevice);

void onMusicDetectionTimerExpired(void*)
{
  stateMachine.onMusicDetectionTimerExpired();
}

void onSpikeDetectionTimerExpired(void*)
{
  stateMachine.onSpikeDetectionTimerExpired();
}

void onMusicStoppingTimerExpired(void*)
{
  stateMachine.onMusicStoppingTimerExpired();
}

void checkTestMode()
{
  if (digitalRead(TestPin) == 0)
    musicStoppingTimer.setDuration(NoMusicDetectionTimeoutTestMode);
  else
    musicStoppingTimer.setDuration(NoMusicDetectionTimeout);
}

void setup() 
{
  Serial.begin(9600);
  outputDevice.setup();
  outputDevice.blinkLedsOnOff(LedBlinkTimeOnStartup); // shows that the processor is alive
  analogReference(INTERNAL);  // Configures the reference voltage to 1.1V used for analog input
  pinMode(TestPin, INPUT);
  Serial.println("setup ready");
}

void loop() 
{
  if (audioSignal.isDetected())
    stateMachine.onAudioDetected();

  musicDetectionTimer.update();
  spikeDetectionTimer.update();
  musicStoppingTimer.update();

  checkTestMode();
}

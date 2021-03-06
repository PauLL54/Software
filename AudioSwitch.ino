//  Copyright © 2019 Paul Langemeijer. All rights reserved.
#include "StateMachine.h"
#include "OutputDevice.h"
#include "AudioSignal.h"
#include "OneshotTimer.h"

const int LedPinOn    =  2; // PD2 pin 4
const int LedPinOff   =  3; // PD3 pin 5
const int SsrPin      =  8; // PB0 pin 14
const int PinJ1       =  5; // PB5 pin 11   Sensitivity
const int PinJ2       =  6; // PB6 pin 12   Sensitivity
const int TestPin     = 11; // PB3 pin 17   when set, selects NoMusicDetectionTimeoutTestMode
const int PinAdcRef   = 14; // PC0 pin 23
const int PinAdc      = 15; // PC1 pin 24
const int Treshold    = 10; // treshold value (in bits) for audio detected

const int LedBlinkTimeOnStartup = 5; // seconds

const unsigned long MusicDetectionTimeout           =    600; // ms 
const unsigned long SpikeDetectionTimeout           =    150; // ms 
const unsigned long NoMusicDetectionTimeout         = 120000; // 2 minutes
const unsigned long NoMusicDetectionTimeoutTestMode =   2000; // 2 seconds 
int m_sensitivity = -1;

// Required forward declarations:
void onMusicDetectionTimerExpired(); 
void onSpikeDetectionTimerExpired();
void onMusicStoppingTimerExpired();

OneshotTimer musicDetectionTimer = OneshotTimer(MusicDetectionTimeout,   onMusicDetectionTimerExpired);
OneshotTimer spikeDetectionTimer = OneshotTimer(SpikeDetectionTimeout,   onSpikeDetectionTimerExpired);
OneshotTimer musicStoppingTimer  = OneshotTimer(NoMusicDetectionTimeout, onMusicStoppingTimerExpired);

OutputDevice outputDevice = OutputDevice(LedPinOn, LedPinOff, SsrPin);
AudioSignal  audioSignal  = AudioSignal(PinAdc, PinAdcRef, Treshold);
StateMachine stateMachine = StateMachine(musicDetectionTimer, spikeDetectionTimer, musicStoppingTimer, outputDevice);

void onMusicDetectionTimerExpired()
{
  stateMachine.onMusicDetectionTimerExpired();
}

void onSpikeDetectionTimerExpired()
{
  stateMachine.onSpikeDetectionTimerExpired();
}

void onMusicStoppingTimerExpired()
{
  stateMachine.onMusicStoppingTimerExpired();
}

void checkSensitivityMode()
{
  int sensitivity = digitalRead(PinJ1);
  sensitivity += digitalRead(PinJ2) << 1;
  sensitivity++; // = 1, 2, 3, 4
  
  if (m_sensitivity != sensitivity)
  {
    m_sensitivity = sensitivity;
    audioSignal.SetTreshold(sensitivity * Treshold);
  }
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
  //Serial.println("setup");
  outputDevice.setup();
  outputDevice.blinkLedsOnOff(LedBlinkTimeOnStartup); // shows that the processor is alive
  analogReference(INTERNAL);  // Configures the reference voltage to 1.1V used for analog input
  pinMode(TestPin, INPUT);
}

void loop() 
{
  if (audioSignal.isDetected())
    stateMachine.onAudioDetected();

  musicDetectionTimer.update();
  spikeDetectionTimer.update();
  musicStoppingTimer.update();

  checkTestMode();
  checkSensitivityMode();
}

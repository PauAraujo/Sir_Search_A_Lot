/**
 * This ESP32 is used as a bluetooth classic middleman for voiceControl
 */

#include <arduino-timer.h>

// Global variables
auto timer = timer_create_default();

#include "VoiceCommands.h"

void setup() {
  Serial.begin(9600);
  voiceCommandSetup();
}

void loop() {
  timer.tick();
  listenToVoiceCommands();
}

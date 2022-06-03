// Include global libaries
#include <SoftwareSerial.h>
#include <arduino-timer.h>

// Global variables
auto timer = timer_create_default();
boolean hasWon = false;
boolean voiceControlled = false;

// Include dependencies
// Include order is important!
#include "RGBSensor.h"
#include "MP3Player.h"
#include "LedControl.h"
#include "OledScreen.h"
#include "Motor.h"
#include "Celebrate.h"
#include "VoiceCommands.h"
#include "ObstacleAvoidance.h"
#include "Driver.h"
#include "Bluetooth.h"

void setup() {
  // Serial debugger
  Serial.begin(9600);

  // Initial setup
  bluetoothSetup();
  voiceCommandSetup();
  ledSetup();
  motorSetup();
  obstacleAvoidanceSetup();
  oledScreenSetup();
  mp3PlayerSetup();
  RGBSensorSetup();
}

// Loop da loop
void loop() {
  timer.tick();
  
  updateOledScreen();

  if(!hasWon && colorRedDetected() && voiceControlled){
    // We have a winner!
    winner();
    return;
  }

  runBLECommunication();
  listenToVoiceCommand();
  
  if(!handbrake){
    selfDriving();
  }
}

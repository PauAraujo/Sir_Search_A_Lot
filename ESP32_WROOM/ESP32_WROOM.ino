/**
 * This ESP32 is used as a clock and lightshow
 */

#include <arduino-timer.h>

// Global variables
auto timer = timer_create_default();
boolean bluetoothIsConnected = false;

#include "Multicore.h"
#include "LedStrip.h"
#include "LedMatrix.h"
#include "Bluetooth.h"

// By default all code runs on core 1
void setup() {
  Serial.begin(9600);

  // Run BLE on core 0 so it doesn't block the timer and led strip
  runOnCore(bluetoothSetup, "BLE", 0);
  ledMatrixSetup();
  ledStripSetup();
}

void loop() {
  timer.tick();
  runLedStrip();
  runTimer();
}

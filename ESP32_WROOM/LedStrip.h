#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#define DATA_PIN 13
#define NUM_LEDS 288
#define MAX_POWER_MILLIAMPS 600
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

#include "PacificaLedAnimation.h"
#include "RainbowLedAnimation.h"

boolean colorActive = true;
void runBlink(){
  if(colorActive){
    FastLED.clear();
    FastLED.show();
    colorActive = false;
    return;
  }
  
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
  }
  FastLED.show();
  colorActive = true;
}

void runStatic(){
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
  }
  FastLED.show();
}

/**
 * 0 = off
 * 1 = blue wave
 * 2 = orange wave
 * 3 = red wave
 * 4 = blinking red
 * 5 = red
 * 6 = winner
 */
int ledAnimationType = 1;
void setLedAnimationType(int animationType){
  ledAnimationType = animationType;
}

// This function will control which animation / colors are used on the ledstrip.
void runLedStrip(){
  // reset blink
  if(ledAnimationType != 4){
    colorActive = true;   
  }

  if(ledAnimationType == 1){
    // normal
    setColorPallate(0);
    EVERY_N_MILLISECONDS(20) {
      runPacifica();
    }
  }else if(ledAnimationType == 2){
    // midway
    setColorPallate(1);
    EVERY_N_MILLISECONDS(20) {
      runPacifica();
    }
  }else if(ledAnimationType == 3){
    // almost
    setColorPallate(2);
    EVERY_N_MILLISECONDS(20) {
      runPacifica();
    }
  }else if(ledAnimationType == 4){
    // losing
    EVERY_N_MILLISECONDS(1000) {
      runBlink();
    }
  }else if(ledAnimationType == 5){
    EVERY_N_MILLISECONDS(1000) {
      runStatic();
    }
  }else if(ledAnimationType == 6){
    runRainbow();
  }
}

void ledStripSetup(){
  delay(3000); // 3 second delay for boot recovery, and a moment of silence
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setMaxPowerInVoltsAndMilliamps( 5, MAX_POWER_MILLIAMPS);
  
  FastLED.clear();  // clear all pixel data
  FastLED.show();
  //setColorPallate(0);
}

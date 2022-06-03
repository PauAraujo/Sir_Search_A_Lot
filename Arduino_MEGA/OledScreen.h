#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ScreenBitmaps.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 useOLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void oledScreenSetup(){
  if(!useOLED.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Oled screen not found, check conncetion");
    return;
  }
  
  useOLED.clearDisplay();
  // Draw bitmap on the screen
  useOLED.drawBitmap(0, 0, bmpMouth, 128, 64, 1); 
  useOLED.display();
}

// 0 -> 2, the current bitmap shown while talking
int activeBmp = 0;
void moveMouth(){
  // Draw bitmap on the screen
  useOLED.clearDisplay();
  useOLED.drawBitmap(0, 0, bmpsTalking[activeBmp], 128, 64, 1); 
  useOLED.display();

  if(activeBmp == 2){
    activeBmp = 0;
    return;
  }
  
  activeBmp++;
}

Timer<>::Task talkInterval;
boolean isTalking = false;
void startTalking(){
  isTalking = true;
  talkInterval = timer.every(100, moveMouth);
}

void stopTalking(){
  if(isTalking){
    timer.cancel(talkInterval);
    isTalking = false;
    activeBmp = 0;
  }

  // Draw bitmap on the screen
  useOLED.clearDisplay();
  useOLED.drawBitmap(0, 0, bmpMouth, 128, 64, 1); 
  useOLED.display();
}

void updateOledScreen(){
  if(!isPlayingMP3() && isTalking) stopTalking();
  else if(isPlayingMP3() && !isTalking) startTalking();
}

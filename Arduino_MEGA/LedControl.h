// Fancy light show
// Documentation:
// https://github.com/FastLED/FastLED/wiki/Basic-usage
#include <FastLED.h>
#define LED_PIN 4
#define LED_TOTAL 1
#define LED_BRIGHTNESS 20
CRGB leds[LED_TOTAL];

// Black = off
String ledLight = "off";
void changeLED(String color){
  if(color == "red") leds[0] = CRGB::Red;
  else if(color == "blue") leds[0] = CRGB::Blue;
  else if(color == "orange") leds[0] = CRGB::Orange;
  else if(color == "green") leds[0] = CRGB::Green;
  else if(color == "black" || color == "off") leds[0] = CRGB::Black;
  else if(color == "purple") leds[0] = CRGB::Purple;

  ledLight = color;
  FastLED.show();
}

void ledSetup(){
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, LED_TOTAL);
  FastLED.setBrightness(LED_BRIGHTNESS);
  changeLED("off");
}

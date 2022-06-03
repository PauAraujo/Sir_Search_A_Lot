// First tried with the official Adafruit_TCS34725 libary, however this seems to be using a delay
// and blocks other code from running correctly, now we are using a library that runs async
// https://github.com/ysard/TCS34725

#include <tcs34725.h>
TCS34725 useRGBSensor;

int RGBValues[3];
boolean colorRedDetected(){
  // Based on testing the red returns 200-210 with a red paper
  // Due to the led light, green and blue are also detected, but they stay concistently below 40
  if(RGBValues[0] > 200 && RGBValues[1] < 40 && RGBValues[2] < 40){
    return true;
  }

  return false;
}

boolean setRGB(void*){
  boolean RGBstatus = useRGBSensor.updateData(true);
  if (RGBstatus) {
    // Convert values to RGB (0-255)
    // https://github.com/jorgemvc/SensorRGB/blob/master/SensorRGB.cpp
    float red=useRGBSensor.r_raw; red/=useRGBSensor.c_raw; red*=256;
    float green=useRGBSensor.g_raw; green/=useRGBSensor.c_raw; green*=256;
    float blue=useRGBSensor.b_raw; blue/=useRGBSensor.c_raw; blue*=256;
    
    // return rgb values with a max of 255
    if(red > 255) RGBValues[0] = 255;
    else RGBValues[0] = floor(red);

    if(green > 255) RGBValues[1] = 255;
    else RGBValues[1] = floor(green);

    if(blue > 255) RGBValues[2] = 255;
    else RGBValues[2] = floor(blue);
  }
  
  return true;
}

void RGBSensorSetup(){
  if (!useRGBSensor.begin()) {
    Serial.println("RGBSensor not found ... check your connections");
    return;
  }
  
  // Set persistence filter to generate an interrupt for every RGB Cycle,
  // regardless of the integration limits
  useRGBSensor.tcs.write8(TCS34725_PERS, TCS34725_PERS_NONE);
  // Set the RGB every 500ms
  timer.every(500, setRGB);
}

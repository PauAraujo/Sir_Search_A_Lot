#include <MD_Parola.h>
#include <MD_MAX72xx.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

// Defining size, and output pins
#define TOTAL_MATRIX_DEVICES 4
#define MATRIX_CLK_PIN 14
#define MATRIX_CS_PIN 15
#define MATRIX_LED_DATA_PIN 12

// Create a new instance of the MD_Parola class
MD_Parola useMatrixBoard = MD_Parola(HARDWARE_TYPE, MATRIX_LED_DATA_PIN, MATRIX_CLK_PIN, MATRIX_CS_PIN, TOTAL_MATRIX_DEVICES);

Timer<>::Task ledMatrixTimer;
int totalCount = 30;
int countDown = totalCount; // total time in seconds
boolean timerRunning = false;
void resetTimer(){
  // Set start time back to 300 seconds and reset the ledStrip animation
  countDown = totalCount;
  timerRunning = false;
}

String getCurrentTime(){
  // Separate by 60 seconds
  float calculatedTime = countDown / 60;
  // Round down to get minutes
  int minutesLeft = floor(calculatedTime);
  // calculate the seconds
  int secondsLeft = countDown - (minutesLeft * 60);

  // convert minutes & seconds to string with int to remove the decimal
  String minutes = String(minutesLeft);
  String seconds = String(secondsLeft);

  // add 0 before single character
  if(minutes.length() == 1){
    minutes = "0" + minutes;
  }
  if(seconds.length() == 1){
    seconds = "0" + seconds;
  }

  String currentTime = minutes + ":" + seconds;
  // add a dot at the end to indicate there is no bluetooth connection
  if(!bluetoothIsConnected){
    return currentTime + ".";
  }

  return currentTime;
}

void stopTimer(){
  if(ledMatrixTimer) timer.cancel(ledMatrixTimer);
  timerRunning = false;

  // return command to inform the arduino the timer started
  Serial2.println("07");
}

void resetMatrixBoard(){
  setLedAnimationType(1);
  resetTimer();
  useMatrixBoard.print(getCurrentTime());
}

// We use numbers so the data send through bluetooh is as small as possible.
// This helps us prevent data loss
void handleTimeLeft(){
  if(countDown == 20){
    Serial2.println("01");
    setLedAnimationType(2);
  }else if(countDown == 15){
    Serial2.println("02");
    setLedAnimationType(3);
  }else if(countDown == 10){
    Serial2.println("03");
  }else if(countDown == 7){
    Serial2.println("04");
    setLedAnimationType(4);
  }else if(countDown == 0){
    Serial2.println("05");
    stopTimer();
    setLedAnimationType(5);
    
    // reset everything after 5s
    delay(5000);
    resetMatrixBoard();
  }
}

boolean matrixBoardCounter(void*){
  // Fetch time as a string we can output to the ledMatrix
  countDown--;
  String timePassed = getCurrentTime();
  // Print time to the ledMatrix.
  useMatrixBoard.print(timePassed);
  handleTimeLeft();

  return true;
}

boolean startTheTimer = false;
void runTimer(){
  if(!startTheTimer){
    return;
  }
  
  if(!timerRunning){
    // reset the time
    resetTimer();
  }

  // Cancel old timer if its exists
  if(ledMatrixTimer) timer.cancel(ledMatrixTimer);

  // Start the timer
  ledMatrixTimer = timer.every(1000, matrixBoardCounter);
  timerRunning = true;
  startTheTimer = false;

  // return command to inform the arduino the timer started
  Serial2.println("06");
}

void ledMatrixSetup(){
  // Intialize the matrix
  useMatrixBoard.begin();
  // Set the intensity (brightness) of the display (0-15)
  useMatrixBoard.setIntensity(10);
  // Clear the display
  useMatrixBoard.displayClear();
  // Align all text to the right
  useMatrixBoard.setTextAlignment(PA_CENTER);
  // Set board to initial values
  resetMatrixBoard();
}

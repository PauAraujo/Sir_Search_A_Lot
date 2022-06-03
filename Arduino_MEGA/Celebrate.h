#define DANCE_MOTOR_SPEED 50

boolean isDancing = false;
boolean danceMoves(void*){
  // Total length of the dance is 4s
  // Song is 14s -> 14/4=3.5 so we can repeat the dance 4 times
  for(int i=0; i<=3; i++){
    changeLED("blue"); // Blue light on led
    drive(1, 400, DANCE_MOTOR_SPEED); // Move forward
    drive(0); // stop
    drive(-1, 400, DANCE_MOTOR_SPEED); // Move backward
    changeLED("orange"); // Orange light on led
    drive(2, 400, DANCE_MOTOR_SPEED); // Move left
    drive(3, 400, DANCE_MOTOR_SPEED); // Move right
    changeLED("green"); // Green light on led
    drive(1, 400, DANCE_MOTOR_SPEED); // Move forward
    drive(-1, 400, DANCE_MOTOR_SPEED); // Move backward
    changeLED("purple"); // Purple light on led
    if(i == 0 || i == 2){
      drive(2, 1000, DANCE_MOTOR_SPEED); // Circle left
      changeLED("red"); // Red light on led
      drive(3, 1000, DANCE_MOTOR_SPEED); // Circle right
      changeLED("black"); // Turn light off
      continue;
    }

    drive(3, 1000, DANCE_MOTOR_SPEED); // Circle right
    changeLED("red"); // Red light on led
    drive(2, 1000, DANCE_MOTOR_SPEED); // Circle left
    changeLED("black"); // Turn light off
  }

  drive(0); // stop
  isDancing = false;
  
  return false;
}

void robotDance(int timeDelay = 0){
  isDancing = true;
  playMP3(10, timeDelay); // Play dance song
  timer.in(timeDelay, danceMoves);
}

void winner(){
  hasWon = true; // Set global haswon variable
  voiceControlled = false; // We are no longer voice controlled
  Serial2.println("2"); // Send winner to the timer
  drive(1, 100, MOTOR_SPEED_SLOW); // Drive a tiny bit further, so we are further on the plate
  drive(0); // stop
  playMP3(8); // Victory sound
  playMP3(9, 3500); // We did it!
  robotDance(5000); // Let's dance!
  playMP3(13, 18000); // Time for a new round. Give me directions when you are ready.
}

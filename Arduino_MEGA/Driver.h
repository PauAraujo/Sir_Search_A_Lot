// Randomize direction to drive to
void decideLeftRight(int time, int speed=MOTOR_SPEED){
  int dir = avoidObstacles(true);
  drive(dir, time, speed);
}

// Randomize direction to drive to
void decideForwardBackward(int time, int speed=MOTOR_SPEED){
  int randomDirInt = random(5); // 0 - 4
  if(randomDirInt != 0){
    // 80% chance to move forward
    if(avoidObstacles() == 1){
      drive(1, time, speed);
    }
  } else {
    // 20% chance to move backward a bit
    drive(-1, time, speed);
    decideLeftRight(time, speed);
  }
}

// Function triggered by timer, needs to return false, else it runs again
// https://github.com/contrem/arduino-timer
boolean wasLost = false;
boolean resetWasLost(){ 
  wasLost = false; return false; 
 }

// Mr Robot sometimes gets lost
boolean iAmLost(){
  // If we were lost just now, do nothing
  // How can we get lost when we are controller by voice?
  if(wasLost || voiceControlled){
    return false;
  }

  // We are lost... Let's stop for a moment
  drive(0);

  // Look around and go to a random direction
  decideLeftRight(300, 100);
  // Stop
  drive(0);
  
  playMP3(12); // Oh no, where do i go now?
  
  // Look around and go to a random direction
  decideLeftRight(300, 100);
  // Stop
  drive(0);
  
  // Look around and go to a random direction
  decideForwardBackward(300, 100);

  // wait 1 minute before we can get lost again
  wasLost = true;
  timer.in(60000, resetWasLost);
 
  return true;
}

// I CAN DRIVE ALL BY MYSELF!
// When -2 is returned we do nothing
void selfDriving(){
  if(isDancing) return;
  
  int dir = avoidObstacles();
  if(dir == -2) return;
  if(dir != 1){
    if(dir == 0){
      drive(-1, 1000, MOTOR_SPEED);
      decideLeftRight(500, MOTOR_SPEED_HIGH);
      return;
    }

    drive(dir, 500, MOTOR_SPEED_HIGH);
    return; 
  }

  if(!iAmLost()) drive(1);
}

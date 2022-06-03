// ULTRASONIC
#define MAX_DISTANCE 30 // max distance in cm
#define ULTRASONIC_TRIGGER 13
#define ULTRASONIC_ECHO 12

// SERVO (Move ultrasonic left / right)
#include <Servo.h>
Servo useServo;
#define SERVO 10

void obstacleAvoidanceSetup(){
  // PIN setup ultrasonic
  pinMode(ULTRASONIC_TRIGGER, OUTPUT);
  pinMode(ULTRASONIC_ECHO, INPUT);

  // INIT SERVO
  useServo.attach(SERVO);
  useServo.write(90);
}

// Servo
// 0 = forward, 1 = left, 2 = right
int lookingInDirection = 0;

// look forward
void lookForward(boolean wait=true){
  if(lookingInDirection == 0){
    return;
  }

  useServo.write(90);
  if(wait){
    // Delay 10ms per degrees
    delay(900);
  }
  
  lookingInDirection = 0;
}

void lookLeft(boolean wait=true){
  if(lookingInDirection == 1){
    return;
  }

  useServo.write(180);
  if(wait){
    // Delay 10ms per degrees
    if(lookingInDirection == 2) delay(1800);
    else if(lookingInDirection == 0) delay(900);
  }
  lookingInDirection = 1;
}

void lookRight(boolean wait=true){
  if(lookingInDirection == 2){
    return;
  }
  
  useServo.write(0);
  if(wait){
    // Delay 10ms per degrees
    if(lookingInDirection == 1) delay(1800);
    else if(lookingInDirection == 0) delay(900);
  }
  lookingInDirection = 2;
}

// Ultrasonic
boolean detectObstacle(){
  // Start ultra sonic detection
  digitalWrite(ULTRASONIC_TRIGGER, LOW);
  // We need to leave this running for 2 microseconds
  // https://create.arduino.cc/projecthub/abdularbi17/ultrasonic-sensor-hc-sr04-with-arduino-tutorial-327ff6#about-project
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIGGER, HIGH);
  // We need to leave this running for 10 microseconds
  // https://create.arduino.cc/projecthub/abdularbi17/ultrasonic-sensor-hc-sr04-with-arduino-tutorial-327ff6#about-project
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIGGER, LOW);

  // Get back the total duration
  long duration = pulseIn(ULTRASONIC_ECHO, HIGH);
  // Speed of sound wave divided by 2 (back and forth)
  int distance = duration * 0.034 / 2;
  
  // Was there an obstacle?
  return distance < MAX_DISTANCE;
}

// Predefine avoidObstacles so the randomlyDecideTimout can use it.
// It will still be able to trigger the whole fn normally
int avoidObstacles(boolean force=false, boolean byPassWait=false);

// If we are voice controlled, stuck and no command is given, decide randomly after 10s where to go
boolean randomlyDecideTimout(void*){
  if(!isStuckAndWaitingForCommand){
    return false;
  }
  
  isStuckAndWaitingForCommand = false;
  int directionToGo = avoidObstacles(false, true);
  drive(directionToGo, 500);
  
  return false;
}

// Lets avoid obstacles, we always look left & right before deciding
// If both options are possible, we pick 1 at random
// 0 is stuck, 1 is forward, 2 is left, 3 is right
int avoidObstacles(boolean force=false, boolean byPassWait=false){
  // When we are currently waiting for a command because we are stuck, do nothing.
  if(isStuckAndWaitingForCommand && !byPassWait){
    return;
  }
    
  boolean hasObstacle = detectObstacle();
  if(force || hasObstacle){
    drive(0);

    // Use random number to figure out where to look
    int randomInt = random(2, 4); // random between 2 and 3

    // We always look left AND right
    boolean hasLeftObstacle = false;
    boolean hasRightObstacle = false;

    if(randomInt == 2){
      lookLeft();
      if(detectObstacle()) hasLeftObstacle = true;
      lookRight();
      if(detectObstacle()) hasRightObstacle = true;
    }else if(randomInt == 3){
      lookRight();
      if(detectObstacle()) hasRightObstacle = true;
      lookLeft();
      if(detectObstacle()) hasLeftObstacle = true;
    }

    // look forward again, without delaying code execution
    lookForward(false);

    // Let's see if there is a command telling us where to go, before we actually move into a direction
    if(listenToVoiceCommand()){
      return;
    }
    
    // stuck
    if(hasLeftObstacle && hasRightObstacle) return 0;
    // left
    else if(!hasLeftObstacle && hasRightObstacle) return 2;
    // right
    else if(hasLeftObstacle && !hasRightObstacle) return 3;
    // both are available
    else {
      // Always return random int if the obstacle detection was forced
      if(byPassWait || force || !voiceControlled){
        return randomInt;
      }
      
      // When both are possible and we are voice controlled we wait and ask..?
      // We are now waiting for a voice command
      isStuckAndWaitingForCommand = true;
      playMP3(11); // Help me, i am stuck.
      // go to random direction after 10s
      timer.in(10000, randomlyDecideTimout);
      // Return -2 so it doesnt do anything.
      return -2;
    }
  }

  return 1;
}

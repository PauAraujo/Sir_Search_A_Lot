// MOTOR globals
#define MOTOR_SPEED_SLOW 40
#define MOTOR_SPEED 70
#define MOTOR_SPEED_HIGH 100
#define MOTOR_STANDBY 3
#define MOTOR_RIGHT 5
#define MOTOR_RIGHT_DIR 7
#define MOTOR_LEFT 6
#define MOTOR_LEFT_DIR 8

void motorSetup(){
  pinMode(MOTOR_LEFT, OUTPUT);
  pinMode(MOTOR_RIGHT, OUTPUT);
  pinMode(MOTOR_LEFT_DIR, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR, OUTPUT);
  pinMode(MOTOR_STANDBY, OUTPUT);
}

// Motor
boolean handbrake = true;
void stopMotor(boolean brake=true){
  // Stop the wheels
  digitalWrite(MOTOR_STANDBY, LOW);
  // Set speed to the wheels
  analogWrite(MOTOR_RIGHT, 0);
  analogWrite(MOTOR_LEFT, 0);

  handbrake = brake;
}

void startMotor(int speed){
  digitalWrite(MOTOR_STANDBY, HIGH);

  // Set speed to the wheels
  analogWrite(MOTOR_RIGHT, speed);
  analogWrite(MOTOR_LEFT, speed);

  handbrake = false;
}

void goForward() {
  // Move all wheels forward
  digitalWrite(MOTOR_LEFT_DIR, HIGH);
  digitalWrite(MOTOR_RIGHT_DIR, HIGH);
}

void goBackward() {
  // Move all wheels backward
  digitalWrite(MOTOR_LEFT_DIR, LOW);
  digitalWrite(MOTOR_RIGHT_DIR, LOW);
}

void goLeft() {
  // Move left wheels backward, and right wheels forward
  digitalWrite(MOTOR_LEFT_DIR, LOW);
  digitalWrite(MOTOR_RIGHT_DIR, HIGH);
}

void goRight() {
  // Move left wheels forward, and right wheels backward
  digitalWrite(MOTOR_LEFT_DIR, HIGH);
  digitalWrite(MOTOR_RIGHT_DIR, LOW);
}

/**
 * This function will control the driving
 * -2 = do nothing
 * -1 = backward
 * 0 = stop
 * 1 = forward
 * 2 = left
 * 3 = right
 */

void drive(int dir, int timer=0, int speed=MOTOR_SPEED) {
  if(dir == -2) return;
  if(dir == -1) goBackward();
  else if(dir == 0) return stopMotor();
  else if(dir == 1) goForward();
  else if(dir == 2) goLeft();
  else if(dir == 3) goRight();

  startMotor(speed);

  // Wait a moment
  if(timer > 0){
    delay(timer);
    stopMotor(false);
  }
}

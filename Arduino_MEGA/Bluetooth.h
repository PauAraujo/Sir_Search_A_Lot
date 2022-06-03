#define BT_GROUND_PIN 50
#define BT_CONNECTED_PIN 53

boolean isBluetoothConnected(){
  return digitalRead(BT_CONNECTED_PIN) == 1;
}

boolean blinkLed(void*){
  if(ledLight == "off") changeLED("blue");
  else changeLED("off"); 
   
  return true;
}

boolean blueToothTimerRunning = false;
Timer<>::Task bluetoothTimer;
boolean checkBluetoothConnection(){
  if(!isBluetoothConnected()){
    if(!blueToothTimerRunning){
      bluetoothTimer = timer.every(1000, blinkLed);
      blueToothTimerRunning = true;
    }
    return false;
  }

  if(ledLight != "blue") changeLED("blue");
  if(blueToothTimerRunning) timer.cancel(bluetoothTimer);
  return true;
}

// Serial event, communicates with the Bluetooth Module
String getSerialString() {
  String serialInput = "";
  serialInput.reserve(200); // reserve 200 bytes
  if(!Serial2.available()){
    return serialInput;
  }

  int startMillis = millis();
  boolean endOfLine = false;
  while (!endOfLine) {
    int currentMillis = millis();
    int seconds = (currentMillis - startMillis) / 1000;
    // Stop listening after 1 seconds
    if(seconds >= 1){
      // Took to long, lets retry.
      Serial.println("Message took to long...");
      serialInput = "";
      endOfLine = true;
    }

    if(!isBluetoothConnected()){
      serialInput = "";
      endOfLine = true;
    }
    
    // Only catch actual characters
    if(!Serial2.available()){
      continue;
    }
    
    // get the new byte:
    char newChar = (char)Serial2.read();
    // add it to the inputString:
    serialInput += newChar;

    // stop while loop when a line character is found
    if(newChar == '\n'){
      endOfLine = true;
    }
  }

  // remove whitespace
  serialInput.trim();
  return serialInput;
}

// 01 = halfway time
// 02 = 1/5 time
// 03 = 10s left
// 04 = 6s left (playtime of losing sound)
// 05 = lost
// 06 = timer started
// 07 = timer stoped
boolean timerRunning = false;
void handleTimerCommand(String command){
  if(command == "01") playMP3(3); // Don't forget, time is ticking.
  else if(command == "02") playMP3(4); // Running out of time!
  else if(command == "03") playMP3(5); // 10 seconds left!
  else if(command == "04") playMP3(6); // Losing sound
  else if(command == "05"){
    drive(0); // stop driving
    playMP3(7); // We didn't lose the game, we just ran out of time.
    // 5000ms delay
    playMP3(13, 5000); //Time for a new round. Give me directions when you are ready.
    voiceControlled = false;
  }
  else if(command == "06"){ // Started the timer!
    timerRunning = true;
  }else if(command == "07"){ // Stopped the timer!
    timerRunning = false;
  }
}

void bluetoothSetup(){
  pinMode(BT_CONNECTED_PIN, INPUT);
  pinMode(BT_GROUND_PIN, OUTPUT);
  digitalWrite(BT_GROUND_PIN, LOW);
  Serial2.begin(9600);
}

// We use numbers so the data send through bluetooh is as small as possible.
// This helps us prevent data loss
void runBLECommunication() {
  if(!checkBluetoothConnection()){
    return;
  }

  if(voiceControlled && !timerRunning){
    Serial2.println("1");
  }

  String incomingCommand = getSerialString();
  if(incomingCommand == ""){
    return;
  }

  Serial.println(incomingCommand);
  handleTimerCommand(incomingCommand);
}

void voiceCommandSetup(){
  Serial3.begin(9600);
}

// Serial event, communicates with the Bluetooth Module
String getVoiceCommandSerialString() {
  String serialInput = "";
  serialInput.reserve(200); // reserve 200 bytes
  if(!Serial3.available()){
    return serialInput;
  }

  int startMillis = millis();
  boolean endOfLine = false;
  while (!endOfLine) {
    int currentMillis = millis();
    int seconds = (currentMillis - startMillis) / 1000;
    // Stop listening after 1 second
    if(seconds >= 1){
      // Took to long, lets retry.
      Serial.println("Message took to long...");
      serialInput = "";
      endOfLine = true;
    }
    
    // Only catch actual characters
    if(!Serial3.available()){
      continue;
    }
    
    // get the new byte:
    char newChar = (char)Serial3.read();
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

// 11 = forward
// 12 = backward
// 13 = stop
// 14 = left
// 15 = right
// 16 = dance
// 17 = drive (without voiceCommand guidance)
boolean isStuckAndWaitingForCommand = false; // Boolean used in ObstacleAvoidance in case we are stuck.
boolean commandedToStop = false; // Boolean used to determine if we need to keep the handbrake on.
void handleVoiceCommand(String command){
  if(command == "11"){ drive(1, 500, MOTOR_SPEED); commandedToStop=false; }
  else if(command == "12"){ drive(-1, 500, MOTOR_SPEED); drive(0);}
  else if(command == "13"){ drive(0); commandedToStop=true; }
  else if(command == "14") drive(2, 500, MOTOR_SPEED_HIGH);
  else if(command == "15") drive(3, 500, MOTOR_SPEED_HIGH);
  else if(command == "16"){ robotDance(); return; }
  else if(command == "17") handbrake = false;

  // Keep the handbrake on untill we move forward again
  if(command != "11" && commandedToStop){
    handbrake = true;
  }

  // Since we received a command
  // we set the isStuckAndWaitingForCommand to false
  isStuckAndWaitingForCommand = false;
}

// We use numbers so the data send through bluetooh is as small as possible.
// This helps us prevent data loss
boolean listenToVoiceCommand() {
  String incomingCommand = getVoiceCommandSerialString();
  if(incomingCommand == ""){
    return false;
  }

  Serial.println(incomingCommand);
  // Lost connection, play the "I can't here you" MP3
  if(incomingCommand == "01" && voiceControlled) playMP3(14);
  // Got reconnected, play the "Lets go" MP3
  else if(incomingCommand == "02" && voiceControlled) playMP3(2);
  // Voice commands always start with a 1
  else if(incomingCommand.startsWith("1")){
    if(!voiceControlled && incomingCommand != "17" && incomingCommand != "16"){
      // reset the global winner variable
      hasWon = false;
      voiceControlled = true;

      // send start command to timer
      Serial2.println("1");
      playMP3(2); // Let's go!
    }

    handleVoiceCommand(incomingCommand);

    return true;
  }

  return false;
}

#define SirSA_BT_ADDRESS "0035FF0BDAD9"
#define BT_STATE_PIN 4

// Read the BT STATE pin, when connected it is HIGH
boolean isConnected(){ 
  bluetoothIsConnected = digitalRead(BT_STATE_PIN) == HIGH;
  return bluetoothIsConnected;
}

// This function helps communicating with the BT module
// It will run a command and waits up to 1s for a given response
// In some cases we can get similar reponses multiple times
// The index allows you to point out which result should be the correct response.
// http://www.martyncurrey.com/hm-10-bluetooth-4ble-modules/
String sendBTCommand(String command, String response, int timeout = 1000){
  Serial2.print(command);
  
  String serialInput = "";
  serialInput.reserve(200); // reserve 200 bytes
  boolean endOfLine = false;

  // keep track of total times we found the response
  int startMillis = millis();
  while (!endOfLine) {
    int currentMillis = millis();
    int millisPassed = currentMillis - startMillis;
    if(millisPassed >= timeout){
      // Took to long, lets retry.
      Serial.println("Didn't get response...");
      serialInput = "";
      endOfLine = true;
    }
    
    // ignore empty bytes
    if(!Serial2.available()){
      continue;
    }

    // get the new byte:
    char newChar = Serial2.read();
    serialInput += newChar;

    if(serialInput.endsWith(response)){
      endOfLine = true;
    }
  }

  return serialInput;
}

boolean initializedBLE = false;
void initializeBLE(){
  // Reset the BLE module so we can get a fresh state
  String setBLEReset = sendBTCommand("AT+RESET", "OK+RESET");
  if(setBLEReset == ""){
    Serial.println("Unable to RESET BLE device");
    return;
  }

  // give it a moment
  delay(1000);

  
  // We need to stop the led from blinking, the led ON is the state connection that is returned
  // If the led blinks it can return "true" while we are not connected
  String BLEIsBlinking = sendBTCommand("AT+PIO1?", "OK+Get:1");
  if(BLEIsBlinking == ""){
    String setBLELed = sendBTCommand("AT+PIO11", "OK+Set:1");
    if(setBLELed == ""){
      Serial.println("Unable to stop BLE led from blinking");
      return;
    }

    // Restart the BLE module so it can pick up the led change
    String setBLEReset = sendBTCommand("AT+RESET", "OK+RESET");
    if(setBLEReset == ""){
      Serial.println("Unable to RESET BLE device");
      return;
    }

    // wait a moment to reboot
    delay(1000);
  }

  // Disable BLE notifications, else it sents "OK+CONN" when it gets connected
  // Which can return a wrong command when we listen to the serial
  String BLEHasNotifications = sendBTCommand("AT+NOTI?", "OK+Get:0");
  if(BLEHasNotifications == ""){
    String setBLENotifications = sendBTCommand("AT+NOTI0", "OK+Set:0");
    if(setBLENotifications == ""){
      Serial.println("Unable to stop BLE notifications");
      return;
    }

    // Restart the BLE module so it can pick up the led change
    String setBLEReset = sendBTCommand("AT+RESET", "OK+RESET");
    if(setBLEReset == ""){
      Serial.println("Unable to RESET BLE device");
      return;
    }

    // wait a moment to reboot
    delay(1000);
  }

  
  
  String BLEIsMaster = sendBTCommand("AT+ROLE?", "OK+Get:1");
  if(BLEIsMaster == ""){
    //Set the BLE device to master, so we can discover other devices and connect to the timer
    String setRole = sendBTCommand("AT+ROLE1", "OK+Set:1");
    if(setRole == ""){
      Serial.println("Unable to set master role on BLE device");
      return;
    }

    String disableAutoConnect = sendBTCommand("AT+IMME1", "OK+Set:1");
    if(disableAutoConnect == ""){
      Serial.println("Unable to disable auto connect");
      return;
    }

    // Restart the BLE module so it can pick up the led change
    String setBLEReset = sendBTCommand("AT+RESET", "OK+RESET");
    if(setBLEReset == ""){
      Serial.println("Unable to RESET BLE device");
      return;
    }

    // wait a moment to reboot
    delay(1000);
  }

  initializedBLE = true;
}

// Try to connect to the robot, this will try to connect in a loop
void connectToBLEDevice(){
  // initialize and reset the BLE module
  if(!initializedBLE){
    initializeBLE();
  }

  sendBTCommand(String("AT+CO0")+String(SirSA_BT_ADDRESS), "OK+CONNF", 3000);
  isConnected();
}

// Serial event, returns data from bluetooth device while connected
String getSerialInput() {
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
    // Stop listening after 1 second
    if(seconds >= 1){
      // Took to long, lets retry.
      Serial.println("Message took to long...");
      serialInput = "";
      endOfLine = true;
    }
    
    // Break loop if we are not connected
    if(!isConnected()){
      serialInput = "";
      endOfLine = true;
      continue;
    }
    
    // Only catch actual characters
    if(!Serial2.available()){
      continue;
    }
    
    // get the new byte:
    char newChar = Serial2.read();
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

// 1 = start timer
// 2 = winner
void handleBLEResponse(String BLESerialInput){
  if(BLESerialInput == "1"){
    if(timerRunning) return;
    setLedAnimationType(1);
    startTheTimer = true;
  }else if(BLESerialInput == "2"){
    setLedAnimationType(6);
    stopTimer();
    delay(24000);
    resetMatrixBoard();
  }
}

// Try to connect up to 3 times
Timer<>::Task tryToConnectTimer;
int BLEConnectionAttempts = 1;
boolean ignoredConnection = false;
boolean establishBLEConnection(void*){
  if(isConnected()){
    timer.cancel(tryToConnectTimer);
    return false;
  }

  if(BLEConnectionAttempts == 3){
    Serial.println("Unable to connect to BLE after 3 attempts, starting the timer.");
    startTheTimer = true;
    ignoredConnection = true;
    timer.cancel(tryToConnectTimer);
    return false;
  }

  connectToBLEDevice();
  BLEConnectionAttempts++;
  return true;
}

void listenToBluetooth()
{ 
  if(!isConnected() && !ignoredConnection){
    if(!tryToConnectTimer){
      tryToConnectTimer = timer.every(5000, establishBLEConnection);
    }
    
    return;
  }
  
  if(!timerRunning){
    resetMatrixBoard();
  }

  if(!isConnected()){
    // keep trying to reconnect
    connectToBLEDevice();
  }

  // once we are connected, we are no longer gonna ignore the connection
  ignoredConnection = true;
  String BLESerialInput = getSerialInput();
  if(BLESerialInput != ""){
    handleBLEResponse(BLESerialInput);
  }
}

void bluetoothSetup(void*) 
{
    // initialise PIN
    pinMode(BT_STATE_PIN, INPUT);
    // initialise serial connections 
    Serial2.begin(9600);
    delay(2000);
    
    // listen to Bluetooth in a loop
    while(true){
      listenToBluetooth();
      delay(10);
    }
}

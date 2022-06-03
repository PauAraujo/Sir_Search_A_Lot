// This bluetooth module waits and just accepts every connection that comes in

#include <SoftwareSerial.h>
SoftwareSerial SirSA_BT(4, 33);

#include <BluetoothSerial.h>
#define BT_DEVICE_NAME "Sir S.A. voice control"
BluetoothSerial voiceBT;

boolean isConnected = false;
String getBTVoiceCommand() {
  String serialInput = "";
  serialInput.reserve(200); // reserve 200 bytes
  if(!voiceBT.available()) return serialInput;

  int startMillis = millis();
  boolean buildSerialInput = false;
  boolean endOfLine = false;
  while (!endOfLine) {
    int currentMillis = millis();
    int seconds = (currentMillis - startMillis) / 1000;
    if(seconds >= 3){
      // Took to long, lets retry.
      Serial.println("Didn't get a message...");
      serialInput = "";
      endOfLine = true;
    }

    if(!isConnected){
      serialInput = "";
      endOfLine = true;
    }
    
    if(!voiceBT.available()) continue;
    
    // get the new byte:
    char newChar = (char)voiceBT.read();
    
    // When start char is found, we start building the serial input string
    if(newChar == '*'){
      buildSerialInput = true;
      continue;
    }

    // Ignore characters as long as the buildSerialInput is false
    if(!buildSerialInput) continue;

    // When we hit the endChar
    if(newChar == '#'){
      endOfLine = true;
      continue;
    }

    // add it to the inputString:
    serialInput += newChar;
  }

  serialInput.trim();
  return serialInput;
}

void listenToVoiceCommands(){
  if(!isConnected){
    return;
  }
  
  String voiceCommand = getBTVoiceCommand();
  if(voiceCommand == ""){
    return;
  }

  // Sometimes google Voice automatically starts the word with a capital
  // So we change the casing to all lower case.
  voiceCommand.toLowerCase();

  Serial.println(voiceCommand);
  // We use numbers so the data send through bluetooh is as small as possible.
  // This helps us prevent data loss
  if(voiceCommand == "straight") SirSA_BT.println("11");
  else if(voiceCommand == "go") SirSA_BT.println("11");
  else if(voiceCommand == "forward") SirSA_BT.println("11");
  else if(voiceCommand == "reverse") SirSA_BT.println("12");
  else if(voiceCommand == "backward") SirSA_BT.println("12");
  else if(voiceCommand == "back") SirSA_BT.println("12");
  else if(voiceCommand == "stop") SirSA_BT.println("13");
  else if(voiceCommand == "left") SirSA_BT.println("14");
  else if(voiceCommand == "right") SirSA_BT.println("15");
  else if(voiceCommand == "dance") SirSA_BT.println("16");
  else if(voiceCommand == "drive") SirSA_BT.println("17");
}

boolean sendDisconnected(void*){
  SirSA_BT.println("01");
  return true;
}

Timer<>::Task bluetoothFeedbackTimer;
// Handle ESP32 bluetooth events
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/bluetooth/esp_spp.html#_CPPv2N18esp_spp_cb_param_t22spp_srv_open_evt_paramE
void setConnection(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  // connected
  if(event == ESP_SPP_SRV_OPEN_EVT){
    if(bluetoothFeedbackTimer){
      timer.cancel(bluetoothFeedbackTimer);
      SirSA_BT.println("02");
    }
    isConnected = true;
    return;
  }

  // disconnected
  if(event == ESP_SPP_CLOSE_EVT){
    sendDisconnected(NULL);
    bluetoothFeedbackTimer = timer.every(20000, sendDisconnected);
    isConnected = false;
    return;
  }
}

void voiceCommandSetup() 
{
  SirSA_BT.begin(9600);
  voiceBT.begin(BT_DEVICE_NAME);
  voiceBT.register_callback(setConnection);
}

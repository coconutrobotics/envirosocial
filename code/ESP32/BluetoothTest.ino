//Libraries
#include "BluetoothSerial.h"            

//Error messages if BT is not enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//Constants

//Variables

//Objects
BluetoothSerial SerialBT;       

void setup() {
  Serial.begin(115200);
  SerialBT.begin("EnviroSocial"); 
  Serial.println("Bluetooth started"); 
}

void loop() {
  if (SerialBT.available() > 0) {
    String command = SerialBT.readString();
    command.trim();                        
    Serial.print("Command from bluetooth: ");               
    Serial.println(command);               
    if (command == "getdata") { 
      Serial.println("Preparing data");
      SerialBT.println("Data ready");
    }
  }
}

// Program for testing the Gas Sensor
// Upload program to ESP32 and open serial monitor
// Type "getdata" to serial monitor and the ESP32 
// will return the value of gas concentration in 0-100
// Values of 40 and below are normal

//Libraries

//Constants
#define gasPin 25

//Variables
float gas;

//Objects

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readString();
    command.trim();                      
    if (command == "getdata") {          
      Serial.println("Preparing data");
      gas = map(analogRead(gasPin),0,4095,0,100);
      Serial.print("Gas: ");
      Serial.print(gas);
      Serial.println(" %");
    }
  }
}

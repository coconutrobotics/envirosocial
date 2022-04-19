// Program for testing the BME280 sensor
// Upload program to ESP32 and open serial monitor
// Type "getdata" to serial monitor and the ESP32 
// will return the values of temperature, humidity
// pressure and altitude

//Libraries
#include <Wire.h>                       
#include <Adafruit_BME280.h>            


//Constants
#define SEALEVELPRESSURE_HPA (1013.25)  //Pressure at sea level, for BME280

//Variables
bool result_bme;                        //Boolean for checking if BME280 works                 

//Objects
Adafruit_BME280 bme;                    

void setup() {
  Serial.begin(115200);   
  result_bme = bme.begin();
  if (result_bme == false) 
  {
    Serial.println("BME280 does not work! Check wiring!");
  } else {
    Serial.println("BME280 works fine!");  
  }
}

void loop() {
  if (Serial.available() > 0) {  
    String command = Serial.readString();   
    command.trim();                         
    if (command == "getdata") {             
      Serial.println("Preparing data");
      if (result_bme == true) {
        float temperature = bme.readTemperature();
        float pressure = bme.readPressure()/100.0F;
        float humidity = bme.readHumidity();
        float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
        //Temperature
        Serial.print("Temperature: ");                           
        Serial.print(temperature);
        Serial.println(" celcius");
        //Pressure
        Serial.print("Pressure: ");
        Serial.print(pressure);
        Serial.println(" hPa");
        //Humidity
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");
        //Altitude
        Serial.print("Altitude: ");
        Serial.print(altitude);
        Serial.println(" m");
      }
    }
  }
}

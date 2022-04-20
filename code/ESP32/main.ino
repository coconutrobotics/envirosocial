//Program for ESP32 TTGO

//Libraries
#include "BluetoothSerial.h"            //Bluetooth ESP
#include <Wire.h>                       //I2C connection with BME280
#include <Adafruit_BME280.h>            //BME280

//Error messages if Bluetooth is not enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//Constants
#define dustPin 12                      //Data pin for dust sensor                    
#define ledPin 18                       //Led pin for dust sensor
#define noisePin 27                     //Data pin for MEMS microphone
#define gasPin 25                       //Data pin for gas sensor
#define SEALEVELPRESSURE_HPA (1013.25)  //Pressure at sea level (BME280)
#define COV_RATIO 0.2                   //For dust sensor
#define NO_DUST_VOLTAGE 100             //For dust sensor
#define SYS_VOLTAGE 3333                //For dust sensor

//Variables
bool result_bme;                                  //Boolean variable for BME280, checks if BME280 is working 
float temperature, pressure, humidity, altitude;  //Variables for BME280
int adcvalue;                                     //Variable for Dust sensor
float density, voltage;                           //Variables for Dust sensor
const int sampleTime = 50;                        //Time frame for sampling noise (milliseconds)
int noise;                                        //Variable for MEMS microphone
int gas;                                          //Variable for gas sensor

//Objects
BluetoothSerial SerialBT;               //Bluetooth object
Adafruit_BME280 bme;                    //BME280 object

//Functions

//Filter function for Dust sensor
int Filter(int m) {
  static int flag_first = 0, _buff[10], sum;
  const int _buff_max = 10;
  int i;
  
  if(flag_first == 0)
  {
    flag_first = 1;

    for(i = 0, sum = 0; i < _buff_max; i++)
    {
      _buff[i] = m;
      sum += _buff[i];
    }
    return m;
  }
  else
  {
    sum -= _buff[0];
    for(i = 0; i < (_buff_max - 1); i++)
    {
      _buff[i] = _buff[i + 1];
    }
    _buff[9] = m;
    sum += _buff[9];
    
    i = sum / 10.0;
    return i;
  }
}

// Find the Peak-to-Peak Amplitude Function for sound sensor
int findPTPAmp(){
   unsigned long startTime= millis();       //Start of sample window
   unsigned int PTPAmp = 0;         
   unsigned int maxAmp = 0;
   unsigned int minAmp = 4096;
   int micOut;

   while(millis() - startTime < sampleTime) 
   {
      micOut = analogRead(noisePin);
      if( micOut < 5000)                     //prevent erroneous readings
      {
        if (micOut > maxAmp)
        {
          maxAmp = micOut;                   //save only the max reading
        }
        else if (micOut < minAmp)
        {
          minAmp = micOut;                   //save only the min reading
        }
      }
   }
   return map(maxAmp - minAmp,0,4096,0,100); //We map the difference of min and max to 0-100 
}

void setup() {
  pinMode(ledPin,OUTPUT);                       //Set led pin of dust sensor as output         
  digitalWrite(ledPin, LOW);                    //Power off led of dust sensor
  Serial.begin(115200);                         //Begin serial communication for debugging
  SerialBT.begin("EnviroSocial");               //Begin Bluetooth device with name EnviroSocial
  Serial.println("Bluetooth started");          //Debugging
  result_bme = bme.begin();                     //Start BME280 sensor
  if (result_bme == false)                      //Debugging
  {
    Serial.println("BME280 does not work!");
  } else {
    Serial.println("BME280 works fine!");  
  }
}

void loop() {
  if (SerialBT.available() > 0) {                             //If there is data in bluetooth serial
    String command = SerialBT.readString();                   //Read the incoming command from bluetooth
    command.trim();                                           //Trim whitespaces
    Serial.print("Command from bluetooth: ");                 //Debugging      
    Serial.println(command);               
    if (command == "getdata") {                               //If command is getdata  
      Serial.println("Preparing data");                       //Debugging
      if (result_bme == true) {                               //Collect values from sensors if they work
        //BME280
        temperature = bme.readTemperature();                    
        pressure = bme.readPressure()/100.0F;
        humidity = bme.readHumidity();
        altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
        //Gas
        gas = map(analogRead(gasPin),0,4095,0,100);
        //Noise
        noise = findPTPAmp();
        //Dust sensor
        digitalWrite(ledPin, HIGH);
        delayMicroseconds(280);
        adcvalue = analogRead(dustPin);
        digitalWrite(ledPin, LOW);
        adcvalue = Filter(adcvalue);
        voltage = (SYS_VOLTAGE / 4096.0) * adcvalue * 11;
        if(voltage >= NO_DUST_VOLTAGE) {
          voltage -= NO_DUST_VOLTAGE;
          density = voltage * COV_RATIO;
        } else density = 0;
        //Send to bluetooth
        SerialBT.print(temperature);
        SerialBT.print("*");
        SerialBT.print(humidity);
        SerialBT.print("*");
        SerialBT.print(pressure);
        SerialBT.print("*");
        SerialBT.print(altitude);
        SerialBT.print("*");
        SerialBT.print(gas);
        SerialBT.print("*");
        SerialBT.print(noise);
        SerialBT.print("*");
        SerialBT.println(density);
        //Write to Serial for debugging
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" Celcius");
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");
        Serial.print("Pressure: ");
        Serial.print(pressure);
        Serial.println(" hPa");
        Serial.print("Altitude: ");
        Serial.print(altitude);
        Serial.println(" m");
        Serial.print("Gas: ");
        Serial.print(gas);
        Serial.println(" %");
        Serial.print("Noise: ");
        Serial.print(noise);
        Serial.println(" %");
        Serial.print("Particles: ");
        Serial.print(density);
        Serial.println(" mg/m3");
      }
    }
  }
}

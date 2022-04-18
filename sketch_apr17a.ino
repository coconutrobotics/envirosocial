//Inlcude libraries

//For BME sensor
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//For Bluetooth 
#include "BluetoothSerial.h"

//Error messages if BT is not enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//Constants
//For Dust sensor
#define iled = 18;          //Led pin
#define vout = 12;          //Analog in pin
#define COV_RATIO 0.2       //ug/mmm / mv
#define NO_DUST_VOLTAGE 400 //mv
#define SYS_VOLTAGE 5000           
//For BME sensor
#define SEALEVELPRESSURE_HPA (1013.25)
//For sound sensor
#define sound = 27;         //Sound pin
//For gas sensor
#define gas_ain=25;         //gas pin

//Objects
Adafruit_BME280 bme;
BluetoothSerial SerialBT;

//Variables
//For BME Sensor
bool result_bme;
//For Dust sensor
float density, voltage;
int adcvalue;
int ad_value;

//Fuction for dust sensor
int Filter(int m)
{
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


void setup(void)
{
  //Setup pin modes
  pinMode(gas_ain,INPUT);
  pinMode(iled, OUTPUT);
  digitalWrite(iled, LOW);                                     //iled default closed
  Serial.begin(115200);                                      
  result_bme = bme.begin();  
  if (result_bme == false) {
    Serial.println("Ο αισθητήρας BME280 δεν δουλεύει! Ελέγξτε καλωδιώσεις!");
  } else {
    Serial.println("Ο αισθητήρας BME280 δουλεύει!");  
  }
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop(void)
{
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
    String command = BTserial.readString();   //Διαβάζουμε το μήνυμα
    Serial.println(command);                  //Γράφουμε το μήνυμα και στην σειριακή θύρα του υπολογιστή για έλεγχο του προγράμματος
    if (command == "getdata") {               //Αν το μήνυμα που πήραμε είναι το getdata τότε πρέπει να μαζέψουμε τα δεδομένα
  }
  digitalWrite(iled, HIGH);
  delayMicroseconds(280);
  adcvalue = analogRead(vout);
  digitalWrite(iled, LOW);
  
  adcvalue = Filter(adcvalue);

  /*
  covert voltage (mv)
  */
  voltage = (SYS_VOLTAGE / 1024.0) * adcvalue * 11;

  /*
  voltage to density
  */
  if(voltage >= NO_DUST_VOLTAGE)
  {
    voltage -= NO_DUST_VOLTAGE;
    
    density = voltage * COV_RATIO;
  }
  else
    density = 0;
    
  /*
  display the result
  */
  Serial.print("The current dust concentration is: ");
  Serial.print(density);
  Serial.print(" ug/m3\n");  

  ad_value=analogRead(gas_ain);
  Serial.println("Gas leakage");
  Serial.print("ad_value:");
  Serial.print(ad_value*3.3/1024);
  Serial.println("V");
   if (result_bme == true) {
    Serial.print("temperature:");
    Serial.print(bme.readTemperature());
    Serial.print("*C   ");
    Serial.print("pressure:");
    Serial.print(bme.readPressure()/100.0F);
    Serial.print("hPa   ");
    Serial.print("humidity:");
    Serial.print(bme.readHumidity());
    Serial.print("%   ");
    Serial.print("altitude:");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println("m");
  }
    Serial.print("Sound: ");
    Serial.println(analogRead(sound));
  delay(1000);
}

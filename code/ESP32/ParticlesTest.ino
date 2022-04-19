// Program for testing the Dust Sensor
// Upload program to ESP32 and open serial monitor
// Type "getdata" to serial monitor and the ESP32 
// will return the value of particles in mg/m3

//Libraries

//Constants
#define dustPin 12                     
#define ledPin 18                      
#define COV_RATIO 0.2                  
#define NO_DUST_VOLTAGE 100            
#define SYS_VOLTAGE 3333               

//Variables
int adcvalue;             
float density, voltage;   

//Function needed by dust sensor
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


//Objects


void setup() {
  Serial.begin(115200);              
  pinMode(ledPin,OUTPUT);            
  digitalWrite(ledPin, LOW);         
}

void loop() {
  if (Serial.available() > 0) {            
    String command = Serial.readString();  
    command.trim();                        
    if (command == "getdata") {            
      Serial.println("Preparing data");
      digitalWrite(ledPin, HIGH);
      delayMicroseconds(280);
      adcvalue = analogRead(dustPin);
      digitalWrite(ledPin, LOW);
      adcvalue = Filter(adcvalue);
      voltage = (SYS_VOLTAGE / 4096.0) * adcvalue * 11;
      if(voltage >= NO_DUST_VOLTAGE)
      {
        voltage -= NO_DUST_VOLTAGE;
        density = voltage * COV_RATIO;
      } else density = 0;
      Serial.print("Particles: ");
      Serial.print(density);
      Serial.println(" mg/m3");
    }
  }
}

//Libraries

//Constants
#define noisePin 27                       

//Varibales
const int sampleTime = 50;                  //Sample time window in microseconds

//Αντικείμενα

void setup() {
  Serial.begin(115200);                     //Start serial communication for debugging          
}

void loop() {
  if (Serial.available() > 0) {             //Get message
    String command = Serial.readString();   //from serial
    command.trim();                         //Trim whitespaces
    if (command == "getdata") {             //If message is getdata then send output
      Serial.println("Preparing data");
      int noise = findPTPAmp();             //Noise outcome is an integer from 0-100
      Serial.print("sound: ");
      Serial.print(noise);
      Serial.println(" %");
    }
  }
}


// Find the Peak-to-Peak Amplitude Function
int findPTPAmp(){
   unsigned long startTime= millis();       //Start of sample window
   unsigned int PTPAmp = 0;         
   unsigned int maxAmp = 0;
   unsigned int minAmp = 4096;
   int micOut;

   while(millis() - startTime < sampleTime) 
   {
      micOut = analogRead(noisePin);
      Serial.println(micOut);
      if( micOut < 5000)                    //prevent erroneous readings
      {
        if (micOut > maxAmp)
        {
          maxAmp = micOut;                  //save only the max reading
        }
        else if (micOut < minAmp)
        {
          minAmp = micOut;                  //save only the min reading
        }
      }
   }
   return map(maxAmp - minAmp,0,4096,0,100); //We map the difference of min and max to 0-100 
}

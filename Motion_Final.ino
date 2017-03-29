
int calibrationTime = 30;        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 3;    //the digital pin connected to the PIR sensor's output
int ledPin = 13;


/////////////////////////////
//SETUP
void setup(){
  Serial.begin(9600);
  
  //Motion Sensor Setup
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
  //End of motion sensor setup
    
  }

////////////////////////////
//LOOP
void loop(){

  //******************************************Start of Motion Sensor code*********************************

     if(digitalRead(pirPin) == HIGH){
      here:
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         Serial.println("---");
         Serial.print("motion detected at ");                              //Motion is detected here *******************************
         Serial.print(millis()/1000);
         Serial.println(" sec"); 
       //  delay(30000);
         }         
         takeLowTime = true;
       }
     
     if(digitalRead(pirPin) == LOW){       
         //the led visualizes the sensors output pin state
         for(int i=0;i<30;i++)
         {
          delay(1000);
          if(digitalRead(pirPin) == HIGH) goto here;
         }
         digitalWrite(ledPin, LOW);

       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }

        lockLow = true;
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
//       if(!lockLow && millis() - lowIn > pause){  
//           //makes sure this block of code is only executed again after 
//           //a new motion sequence has been detected
//           lockLow = true;                        
//           Serial.print("motion ended at ");      //output
//           Serial.print((millis() - pause)/1000);
//           Serial.println(" sec");
//           digitalWrite(ledPin, LOW);
//           delay(50);
//           }
       }
 



    //****************************************End of motion sensor***************************************



 }    
  
    


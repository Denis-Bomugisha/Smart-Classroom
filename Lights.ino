
int led_green =4;
int led_ac = 5;
int led_red = 11;
int switch_green = 8;
int switch_red = 9;
int state = 0;
bool lastState = LOW;
int state2 = 0;
bool lastState2 = LOW;
String reading;
String R1,R2;
String R3,R4;
String a;
int motion_counter = 0;

//Motion
int calibrationTime = 30;        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 7;    //the digital pin connected to the PIR sensor's output
int ledPin = 13;
//End of Motion


void setup() {
  // put your setup code here, to run once:
pinMode(led_green, OUTPUT);
pinMode(led_ac, OUTPUT);
pinMode(led_red, OUTPUT);
pinMode(switch_green, INPUT);
pinMode(switch_red, INPUT);
Serial.begin(9600);

//Motion Setup
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
//End of Motion Setup

}


void loop() {
  // put your main code here, to run repeatedly:

//Lights Control
  //read status of switch of green led
  state = digitalRead(switch_green);
  
  
  if (state == HIGH)
  {
     lastState = !lastState;
  }

  if (lastState == HIGH)
  {
    digitalWrite(led_green, HIGH);
    R3 = "Light 1 is ON";
    delay(500);
  }
  else if (lastState == LOW)
  {
    digitalWrite(led_green, LOW);
    R3 = "Light 1 is OFF";
    delay(500);
  }
   

    state2 = digitalRead(switch_red);
  
  if (state2 == HIGH)
  {
     lastState2 = !lastState2;
  }

  if (lastState2 == HIGH)
  {
    digitalWrite(led_red, HIGH);
    R4 = "Light 2 is ON";
    delay(500);
  }
  else if (lastState2 == LOW)
  {
    digitalWrite(led_red, LOW);
     R4 = "Light 2 is OFF";
    delay(500);
  }
//End of Lights Control

//Get status of Lights and AC
reading = Serial.readString();

if (reading == "status")
{
  delay(1000);
  Serial.println("---");
  Serial.println(R3);
  Serial.println(R4);
  Serial.println(a);
  Serial.println("---");
  delay(1000);
  
}

else if (reading == "00" || motion_counter>20)
{
  Serial.println(reading);
  
  state = digitalRead(switch_green);
  reading = Serial.readString();
  digitalWrite(led_green, LOW);
   digitalWrite(led_red, LOW);
   
  lastState = LOW;
  lastState2 = LOW;
}

else if (reading == "10")
{
  Serial.println(reading);

  state = digitalRead(switch_green);
  reading = Serial.readString();
  digitalWrite(led_green, HIGH);
  digitalWrite(led_red, LOW);

 
  lastState = HIGH;
  lastState2 = LOW;
  
}

else if (reading == "01")
{
  Serial.println(reading);
 
  state = digitalRead(switch_green);
  reading = Serial.readString();
  digitalWrite(led_red, HIGH);
  digitalWrite(led_green, LOW);

   
  lastState = LOW;
  lastState2 = HIGH;
   //control("100");
}

else if (reading == "11")
{
  Serial.println(reading);

  state = digitalRead(switch_green);
  reading = Serial.readString();
  digitalWrite(led_green, HIGH);
   digitalWrite(led_red, HIGH);

  
  lastState = HIGH;
  lastState2 = HIGH;
   //control("100");
}


else if (reading == "AC OFF" || motion_counter>15)
{
  digitalWrite(led_ac, LOW);
  a = "AC is OFF";
}

else if (reading == "AC ON")
{
  digitalWrite(led_ac, HIGH);
  a = "AC is ON";
}

motion_counter = motion_counter + 1;
Serial.print("Time without Motion: ");
Serial.print(motion_counter);
Serial.println(" ");
   //Motion Code
 if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec"); 
         
         delay(50);
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       
       digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state

       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           Serial.println("---");
           motion_counter = 0;
           delay(50);
           }
       }
   //Motion code
 
}

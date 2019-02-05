#include <SoftwareSerial.h>
#include "Stepper.h"
#include <Servo.h>

Servo myservo;  

// Stepper
const int stepsPerRevolution = 64; 
Stepper myStepper(stepsPerRevolution, 4,6,5,7);

SoftwareSerial BT(8, 9); 
// BT module TX to D8
// BT module RX to D9
char a; // incoming character from other device
const int bt_lamp = 13;

// Buzzer
const int buzzer = 10;

// Smoke sensor
const int smoke = A5; 
int sensorThreshold = 350; 

// Motion sensor
int ledPin = 3;
const int inputPin = 2;
int val = 0;
int pirState = LOW;

void setup()  
{
  myStepper.setSpeed(300); 
  //pinMode(13, OUTPUT);
  pinMode(ledPin, OUTPUT);     
  pinMode(inputPin, INPUT);    
  myservo.attach(11);  
  BT.begin(9600);
  BT.println("Hello from House!");
}

void loop() 
{
  int smokeLev = analogRead(smoke); 
  
  if (BT.available())
  {
    a = BT.read();
    if (a=='3')
    {
      digitalWrite(bt_lamp, HIGH);
      BT.println(smokeLev);      
    }
    if (a=='4')
    {
      // Open Curtain
      myStepper.step(2500); 
      delay(1000);
    }
    if (a=='5')
    {
      // Close curtain
      myStepper.step(-2500); 
      delay(1000);
    } 
  }
  smoke_Servo_buzzer();
  motion_lamp();
}

void smoke_Servo_buzzer()
{
  int pos = 180;    
  int smokeLev = analogRead(smoke); 

  if(smokeLev > sensorThreshold){    
    BT.write(smokeLev);
    myservo.write(pos);   
    for(int a = 0; a<10; a++)
    {
      tone(buzzer, 1000, 200);
      delay(200);
    }
  }
  else
  {
    myservo.write(0);   
    noTone(buzzer);
  }
}

void motion_lamp()
{
  val = digitalRead(inputPin);  
      if (val == HIGH) {           
        digitalWrite(ledPin, HIGH);  
        if (pirState == LOW) {       
          BT.println("Motion detected!");        
          pirState = HIGH;
        }
      } else {
        digitalWrite(ledPin, LOW);
        if (pirState == HIGH){         
          BT.println("Motion ended!");         
          pirState = LOW;
        }
      }
}

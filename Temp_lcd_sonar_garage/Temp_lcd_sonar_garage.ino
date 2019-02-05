#include <DHT.h>
#include <LiquidCrystal.h>
#include "Stepper.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Temp and Hum
#define DHTPIN 1 // Загрузка жасағанда мына пиндегі коннекторды ала тұру керек
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

// Ultrasonic sensor
#define echoPin 12 // Echo Pin
#define trigPin 13 // Trigger Pin

int maximumRange = 100; 
int minimumRange = 0; 
long duration, distance; 

// Stepper Motor for Garage
const int stepsPerRevolution = 64;
Stepper myStepper(stepsPerRevolution, 8,10,9,11);
bool closed = true;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Temperature is: ");
  // Sonar
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Stepper
  myStepper.setSpeed(300);
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print(dht.readTemperature());
  lcd.print("   ");
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  distance = duration/58.2;

  if (distance >= maximumRange)
  {
    lcd.print(1000);
    if(!closed){ // if open close garage
      myStepper.step(3000);
      delay(1000);
      closed = true;
    }
  }
  else {
    lcd.println(distance);
    if(closed){ // if closed open garage
      myStepper.step(-3000); 
      delay(1000);
      closed = false;
    }
  }
  delay(50);
}


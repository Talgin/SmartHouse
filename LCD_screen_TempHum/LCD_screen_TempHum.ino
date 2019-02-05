#include <DHT.h>
#include <LiquidCrystal.h>
#include "Stepper.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Temp and Hum
#define DHTPIN 1 // do not forget to remove the DHT sensor while uploading!!!!!!!!
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

// defining Ultrasonic sensor
#define echoPin 12 // Echo Pin
#define trigPin 13 // Trigger Pin

int maximumRange = 100; // Maximum range needed 
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance

// Stepper Motor for Garage
const int stepsPerRevolution = 64; //кол-во шагов на оборот
Stepper myStepper(stepsPerRevolution, 8,10,9,11);
bool closed = true;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Temperature is: ");
  // Sonar
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Stepper
  myStepper.setSpeed(300); //скорость обороты в минуту
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the temperature
  lcd.print(dht.readTemperature());
  lcd.print("   ");
  //lcd.print(millis() / 1000);  
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
      // Close curtain
      myStepper.step(-2500); //поворот на 2000 шагов
      delay(1000);
      closed = true;
    }
    //digitalWrite(LEDPin, HIGH); 
  }
  else {
    lcd.println(distance);
    if(closed){ // if closed open garage
      // Open Curtain
      myStepper.step(2500); //поворот на 2000 шагов
      delay(1000);
      closed = false;
    }
    //digitalWrite(LEDPin, LOW); 
  }
 
  //Delay 50ms before next reading.
  delay(50);
}


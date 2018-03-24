// Written by Ryan Jones

#include <Servo.h>

const int buttonPin = 2;  //button used to move servo to lock position, attached to digital input 2
int buttonState = 0;  //initial state of button

Servo launch;  // create a servo object to control a servo
Servo wind;
int position = 0; //servo starts at neutral position

void setup() {
  Serial.begin(9600); //display serial input (reads pressure sensor value - adjust threshold accordingly)
  pinMode(buttonPin, INPUT); //make the button pin an input pin
  launch.attach(9); //servo attached to pin 9 to hold/fire catapult
  wind.attach(10);
  launch.write(0);
  wind.write(0);
}

void loop() {
  buttonState = digitalRead(buttonPin); //read value of button
  
  if(buttonState == HIGH) { //if button pressed...
  wind.write(180);
  delay(3000);
  launch.write(180); //...move servo 90 degrees (change this number to fit your personal use)
  delay(5000);
  launch.write(0);
  wind.write(0);
  }

  

}

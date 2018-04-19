#include <Servo.h>

Servo sweepServo;
#define SWEEPPIN 6
int pos = 0;
int fixedpos = 0;

Servo tensionServo;
#define TENSED  180
#define RELAXED 0

Servo triggerServo;
#define BLOCKED 0
#define UNBLOCKED 90

#define BUTTONPIN 2
#define TENSIONPIN 9
#define TRIGGERPIN 10

bool armed = false;
bool sweeping = true;

void setup() {
  // put your setup code here, to run once:
  sweepServo.attach(SWEEPPIN);
  sweepServo.write(90);
  pinMode(BUTTONPIN, INPUT); // init button as input
  tensionServo.attach(TENSIONPIN); // init servo to pin 9
  triggerServo.attach(TRIGGERPIN); // init servo to pin 10
  // See if each servo moves back and forth a few times when initialized (just a check to see if they are in position) 
  tensionServo.write(RELAXED); delay(500); tensionServo.write(TENSED); delay(500); tensionServo.write(RELAXED); delay(500);
  triggerServo.write(UNBLOCKED); delay(500); triggerServo.write(BLOCKED); delay(500); triggerServo.write(UNBLOCKED); delay(500);
  // Initialize builtin LED as an output (will later be used as a check: off when system = unarmed, on when system = armed
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (sweeping) { // Tells the Servo to move unless otherwise directed
    for (pos = 90; pos <= 130; pos += 1) { // goes from 90 degrees to 130 degrees
    // in steps of 1 degree
      sweepServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(55);                       // waits 55ms for the servo to reach the position
      if (digitalRead(BUTTONPIN) == HIGH) {
        sweeping = false;
        delay(500);
        return;
      }
  }
    for (pos = 130; pos >= 90; pos -= 1) { // goes from 130 degrees to 90 degrees
      sweepServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(55);                       // waits 55ms for the servo to reach the position
      if (digitalRead(BUTTONPIN) == HIGH) {
        sweeping = false;
        delay(500);
        return;
      }
  } 
  }
  if (digitalRead(BUTTONPIN) == HIGH) {
    if (armed) {
      // Catapult is armed, fire
      triggerServo.write(UNBLOCKED);
      delay(500);
      tensionServo.write(RELAXED);
      delay(500);
      armed = false;
      sweeping = true;
    } else {
      // Where we arm the catapult
      triggerServo.write(BLOCKED); 
      delay(500);
      tensionServo.write(TENSED);
      delay(500);
      armed = true;
    }
  } else {
    // When button is not pressed, check again in 0.5 sec
    delay(50);
  }
  if (armed) {
    // builtin LED on Arduino lights up if the system is armed
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
    
  }



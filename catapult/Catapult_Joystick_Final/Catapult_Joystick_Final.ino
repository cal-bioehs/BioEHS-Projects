#include <Servo.h>

Servo sweepServo;
#define SWEEPPIN 6

// joystick setup
int joyX = 0;
int joyVal; 

Servo tensionServo;
#define TENSED  180
#define RELAXED 0

Servo triggerServo;
#define BLOCKED 0
#define UNBLOCKED 90

#define BUTTONPIN 4
#define TENSIONPIN 9
#define TRIGGERPIN 10

bool armed = false;

void setup() {
  // put your setup code here, to run once:
  sweepServo.attach(SWEEPPIN);
   pinMode(BUTTONPIN, INPUT_PULLUP); // init button as input
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
  joyVal = analogRead(joyX);
  joyVal = map(joyVal, 0, 1023, 0, 180);
  sweepServo.write(joyVal);
  delay(15);
    if (digitalRead(BUTTONPIN) == LOW) {
    if (armed) {
      // Catapult is armed, fire
      triggerServo.write(UNBLOCKED);
      delay(500);
      tensionServo.write(RELAXED);
      delay(500);
      armed = false;
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

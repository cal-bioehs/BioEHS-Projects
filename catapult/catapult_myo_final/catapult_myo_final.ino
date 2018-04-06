#include <Servo.h>

Servo tensionServo;
#define TENSED  180
#define RELAXED 0

Servo triggerServo;
#define BLOCKED 0
#define UNBLOCKED 90

#define MYOPIN A0
#define TENSIONPIN 9
#define TRIGGERPIN 10

bool armed = false;
const int threshVal = 100;
int sensorValue = 0;  //initialization of sensor variable, equivalent to EMA Y
float EMA_a = 0.3;    //initialization of EMA alpha
int EMA_S = 0;        //initialization of EMA S
int highpass = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(MYOPIN, INPUT); // init button as input
  tensionServo.attach(TENSIONPIN); // init servo to pin 9
  triggerServo.attach(TRIGGERPIN); // init servo to pin 10
  // See if each servo moves back and forth a few times when initialized (just a check to see if they are in position) 
  tensionServo.write(RELAXED); delay(500); tensionServo.write(TENSED); delay(500); tensionServo.write(RELAXED); delay(500);
  triggerServo.write(UNBLOCKED); delay(500); triggerServo.write(BLOCKED); delay(500); triggerServo.write(UNBLOCKED); delay(500);
  // Initialize builtin LED as an output (will later be used as a check: off when system = unarmed, on when system = armed
  pinMode(LED_BUILTIN, OUTPUT);
  EMA_S = analogRead(MYOPIN);     //set EMA S for t=1
}

void loop() {
  // put your main code here, to run repeatedly:
  // check if buttonState is HIGH (button is pressed):
  int sensorValue = analogRead(MYOPIN);
  EMA_S = (EMA_a*sensorValue) + ((1-EMA_a)*EMA_S);  //run the EMA
  highpass = sensorValue - EMA_S;                   //calculate the high-pass signal
  if (highpass > threshVal) 
  {
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
    // When arm is not flexed, check again in 0.5 sec
    delay(500);
  }
  if (armed) {
    // builtin LED on Arduino lights up if the system is armed
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

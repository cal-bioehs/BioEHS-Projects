#include <Filters.h>
float filterFrequency = 10.0;
FilterOnePole lowpassFilter( LOWPASS, filterFrequency );
void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(lowpassFilter.input(analogRead(A0) + 100));
  delay(10);
}

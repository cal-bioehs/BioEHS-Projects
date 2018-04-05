/* thresholds for registering a key press*/
const int thresh1 = 1000;
/*assignment of inputs on MaKey MaKey*/
const int sensor1Pin = A3;
#include "Keyboard.h"
int LED (9);
void setup()
{
pinMode(LED, OUTPUT);
Serial.begin(9600);
}
/*The code below samples the MyoWare every 50 ms, sends its value to the serial port, and if the value is above threshold, sends a keypress. You can sample more frequently by reducing the delay. This will make the values harder to read in the serial window.*/
void loop()
{ int sensor1Val = analogRead(sensor1Pin);
Serial.println(sensor1Val);
if(sensor1Val >= thresh1)
{
Keyboard.begin();

// Sends keypress of letter "c"
Keyboard.write('c');
//wait 50 ms before sampling again
delay(50);
}
else
//wait 50 ms before sampling again
delay(50);
}

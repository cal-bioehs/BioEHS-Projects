//Global Variables
int sensorPin = 0;    //pin number to plug in Myoware
int sensorValue = 0;  //initialization of sensor variable, equivalent to EMA Y
float EMA_a = 0.3;    //initialization of EMA alpha
int EMA_S = 0;        //initialization of EMA S
int highpass = 0;
 
void setup(){
  Serial.begin(9600);              //setup of Serial module, 115200 bits/second
  EMA_S = analogRead(sensorPin);     //set EMA S for t=1
}
 
void loop(){
  sensorValue = analogRead(sensorPin);              //read the sensor value using ADC
  EMA_S = (EMA_a*sensorValue) + ((1-EMA_a)*EMA_S);  //run the EMA
  highpass = sensorValue - EMA_S;                   //calculate the high-pass signal
 
  Serial.println(highpass);
   
  delay(20);                                //20ms delay
}


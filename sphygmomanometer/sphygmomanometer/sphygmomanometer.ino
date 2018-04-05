#include <Button.h>         // Include library for using tactile switch
#include <LiquidCrystal.h>  // Include library for using LCD Screen
 
// Initialize the library with the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int pressPin = A0;      // Pin at the output of the amplified sensor
int pumpPin = 7;        // Pin for controlling the air pump
int pumpCmd = 0;        // Set the pump to 0 at the beginning
float sensorValue = 0;  // Initialize sensor value;
float pressure = 0;     // Initialize converted pressure value
Button button(9);       // Initialize button pin for communication
boolean hasStarted;     // Initialize boolean for button logic
boolean firstTime;
int diastolic;
int systolic;
int meanPressure;
int presMeas[15];
int anaz[15];
int index[8];
int p1;
int p2;
int count;
int maxf;


void setup() 
{ 
  // set up the LCD's number of columns and rows
  lcd.begin(16, 2);
  lcd.clear();
  // Print a message to the LCD
  button.begin();
  pinMode(pumpPin, OUTPUT); // Indicate that controlling the air pump is an output
  pinMode(pressPin, INPUT); // Indicate that reading the pressure is an input
  Serial.begin(9600);       // Initialize communication with serial monitor
  analogWrite(pumpPin, pumpCmd); // Set air pump to 0 speed
  initialize();
} 

void loop() 
{ 
  runProgram();
}


void initialize() {
  lcd.clear();
  lcd.print("Setting Up...");
  analogWrite(pumpPin, pumpCmd);
  hasStarted = false;
  firstTime = true;
  diastolic = 0;
  systolic = 0;
  meanPressure = 0;
  int presMeas[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int anaz[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int index[] = {0, 0, 0, 0, 0, 0, 0, 0};
  count = 0;
  maxf = 0;
  delay(3000);
}
void(* resetFunc) (void) = 0;//declare reset function at address 0
 
void runProgram() {
  if (!button.pressed() && !hasStarted) {
  if (firstTime) {
    lcd.clear();
    lcd.print("Press Button");
    firstTime = false;
    delay(10);
  } else {
    // do nothing
    delay(10);
  }
 } else if(button.pressed() && !hasStarted){
  hasStarted = true;
 } else {
    hasStarted = true;
    cuffPump();
    deflateCuff();
    maxFluct(anaz);
    firstTime = true;
    getIndex();
    int total = 0;
    for (int l = 0; l < 1; l++) {
      total = presMeas[index[l]] + total;
    }
    meanPressure = (total / count) * (200/102);
    diastolic = .85 * meanPressure;
    systolic = .55 * meanPressure;
    lcd.clear();
    lcd.print("MAP: ");
    lcd.print(meanPressure);
    delay(5000);
    lcd.clear();
    lcd.print("Diastolic: ");
    lcd.print(diastolic);
    lcd.setCursor(0,1);
    lcd.print("Systolic: ");
    lcd.print(systolic);
    delay(10000);
    while (!button.pressed()) {
      if (firstTime) {
        lcd.clear();
        lcd.print("Press to");
        lcd.setCursor(0,1);
        lcd.print("reset");
        delay(20);
        hasStarted = false;
        firstTime = false;
      } else if (button.pressed()){
        initialize();
        resetFunc();
      } else {
        delay(10);
      }
    }
 }
}



float getPressure() {
  /*
   * Section to read the analog pressure and 
   * convert to mmHg based on calibration
   */
  sensorValue = analogRead(pressPin);
  //int output = map(sensorValue, 87, 321, 0, 200);
  int output = map(sensorValue, 87, 280, 0, 200);
  return output;
}

void cuffPump() {
  analogWrite(pumpPin, 255);
  while (pressure < 190) {
    pressure = getPressure();
    lcd.clear();
    lcd.print("Inflating...");
    lcd.setCursor(0,1);
    lcd.print(pressure);
    lcd.print(" mmHg");
    delay(10);
  }
  analogWrite(pumpPin, 0);
}

void deflateCuff() {
  while (pressure > 170) {
    lcd.clear();
    lcd.print("Release Pressure");
    lcd.setCursor(0,1);
    lcd.print("Slowly");
    pressure = getPressure();
    delay(20);
  }
  lcd.clear();
  lcd.print("Collecting");
  lcd.setCursor(0,1);
  lcd.print("Results");
  int count = 0;
   for (int i = 0; i < 15; i++) {
      if (i > 8) {
        delay(150);
      } else {
        delay(100);
      }
      pressure = getPressure();
      presMeas[i] = pressure;
      delay(20);
      p1 = getPressure();
      delay(100);
      p2 = getPressure();
      anaz[i] = max(p1,p2);
   }
}

void maxFluct(int array[]) {
  for (int j = 0; j < 15; j++) {
    if (array[j] > maxf) {
      maxf = array[j];
    }
  }
}

void getIndex() {
  for (int k = 5; k < 15; k++) {
    if (anaz[k] == maxf) {
      index[k - 5] = 5;
      count++;
    }
  }
}



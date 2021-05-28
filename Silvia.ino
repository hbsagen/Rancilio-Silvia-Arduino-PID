// Includes:
// - Show current temp
// - Show pump live runtime
// - Flashes pump total runtime for 5 secs after pump stops

#include <PID_v1.h>
#include "Adafruit_MAX31855.h"
#include <movingAvg.h>
#include <Arduino.h>
#include <TM1637TinyDisplay.h>

#define RELAY_PIN 7
#define buttonPin 2

// tempbryter 100 når c = 115
// tempbryter 140 når c = 158

movingAvg avgTemp(5);

Adafruit_MAX31855 thermocouple(10); //MAXCS

double Setpoint, Input, Output, tempoff, temp;
double Kp=150.00, Ki=0.20, Kd=17.00;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

int buttonState = 0;         // variable for reading the pushbutton status
int WindowSize = 5000;
int RELAY_STATE = 0;
int avg, disptemp;
int i = 0;
int done = 0;
unsigned long windowStartTime;

TM1637TinyDisplay display(5, 6); //CLK, DIO

void setup(void) {
  Serial.begin(9600);
  Setpoint = 93.01; //target temp for PID
  tempoff = - 3.00; //difference in temp from boiler temp (negative number increase boiler real temp)
  
  display.setBrightness(BRIGHT_1);
  display.clear();
  display.showString("on",2,1);
  delay(2500);
  display.showString("SP",2,0);
  display.showNumber(int(Setpoint),false,2, 2);
      
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(RELAY_PIN, LOW);

  pinMode(A0, INPUT);
  
  if (!thermocouple.begin()) {
    display.showString("ErrP");
    while (1) delay(10);
  }

  windowStartTime = millis();

  delay(2500);

  myPID.SetOutputLimits(0, WindowSize);
  myPID.SetMode(AUTOMATIC);

  avgTemp.begin();
  Input = 20;

}

void loop(void) {

  double c = thermocouple.readCelsius();
  double d;
   
  if (isnan(c) || c <= 0) {
    d = temp * 100; //Inherit temp from last good reading
    Serial.print(c);
    Serial.print(" - Error: ");
    Serial.print(thermocouple.readError());
    Serial.print(" Inherit: ");
    Serial.println(temp);
  }
  else {
    d = (c + tempoff) * 100;
    temp = c + tempoff; //last good reading
  }
  
  avg = avgTemp.reading(d);
  Input = avg / 100;
  Serial.print("C: ");
  Serial.print(c);
  Serial.print( ",D: ");
  Serial.print(d/100);
  Serial.print(" ,Input ");
  Serial.println(Input);
      
  myPID.Compute();
  
  unsigned long now = millis();
  
  if (now - windowStartTime > WindowSize) { 
    windowStartTime += WindowSize;
  }

  if (Output > now - windowStartTime) {
    digitalWrite(RELAY_PIN, HIGH);
    RELAY_STATE = 1;
  }
  else {
    digitalWrite(RELAY_PIN, LOW);
    RELAY_STATE = 0;
  }
    
  if (getMaxValue() >= 10){ //viser tiden pumpa har gått
    display.showString("", 1, 3);
    display.showNumber(i,false,3,0);
    i = i +1;
    done = 1;
  } 
  else{
    if (done == 1 or done == 3 or done == 5){
      display.clear();
      done = done + 1;
    }
    else if (done == 2 or done == 4){
      display.showNumber(i,false,3,0);
      done = done + 1;
    }
    else{
      i = 0;
      done = 0;
      disptemp = int(Input);
      display.showString("\xB0", 1, 3);        // Degree Mark, length=1, position=3 (right)
      display.showNumber(disptemp, false, 3, 0);    // Number, length=3, position=0 (left)
    }
  } 
    
  delay(770);
}

int getMaxValue() { 
  int sensorValue;
  int sensorMax = 0;
  uint32_t start_time = millis();
  while((millis()-start_time) < 200) { //sample for 200ms 
    sensorValue = analogRead(A0);
    
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }
  }
  return sensorMax;
}

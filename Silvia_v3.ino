#include <PID_v1.h>
#include "Adafruit_MAX31855.h"
#include <movingAvg.h> 

#define RELAY_PIN 7
#define RELAY_PIN_2 40
#define MAXCS   10

//LED fade opp ved oppvarming
//LED fade ned ved kjøling
//LED blinke ved c > 120
//legge in boolean på tempsikring

// tempbryter 100 når c = 115
// tempbryter 140 når c = 158

movingAvg avgTemp(5);

Adafruit_MAX31855 thermocouple(MAXCS);

double Setpoint, Input, Output, prevtemp, Steamtemp, tempoff;
double Kp=150.00, Ki=0.20, Kd=17.00; // 48.86 - 0.33 - 5.00 OK
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

int WindowSize = 5000;
int RELAY_STATE = 0;
int RELAY_STATE_2 = 0;
int Status = 0; // 1 - Error thermocouple, 10 - myPID.SetMode(MANUAL), 100 - something
int avg;

unsigned long windowStartTime;
unsigned long lampTime;
unsigned long serialTime;

void setup(void) {

  Setpoint = 100.01; //target temp for PID
  Steamtemp = 140.01; //indicator off temp for steam
  tempoff = - 10.00; //difference in temp from boiler temp (negative number increase boiler real temp)
    
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(RELAY_PIN_2, HIGH);
  
  Serial.println("Initializing sensor...");
  if (!thermocouple.begin()) {
    Serial.println("ERROR.");
    while (1) delay(10);
  }

  windowStartTime = millis();
  lampTime = millis();

  delay(5000);
  myPID.SetOutputLimits(0, WindowSize);

  myPID.SetMode(AUTOMATIC);
  Serial.println("Input avg, Boiler , Lamp , Output , c , Status");
  serialTime = millis();

  avgTemp.begin();
}

void loop(void) {

   double c = thermocouple.readCelsius();
   
  if (isnan(c)) {
    Status = Status + 1;
    c = Input;
  }
  else {
    c = (c + tempoff) * 100;
    avg = avgTemp.reading(c);
    Input = avg / 100;
    c = c / 100;
  }
  
  if ( Input >= 120) {
    myPID.SetMode(MANUAL);
    Output = 0;
    Status = Status + 10;
  }
    
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

    Serial.print(Input);
    Serial.print(" , ");
    Serial.print(RELAY_STATE);
    Serial.print(" , ");
    Serial.print(RELAY_STATE_2);
    Serial.print(" , ");
    Serial.print(Output);
    Serial.print(" , ");
    Serial.print(c);
    Serial.print(" , ");
    Serial.println(Status);

//lamp control:

  if (millis() - 10000 >= lampTime) {
    if (millis() >= 900000) {
      if (Input >= 97) {
        if ((Input <= 103) || (Input >= Steamtemp)) {
          digitalWrite(RELAY_PIN_2, LOW);
          RELAY_STATE_2 = 2;
        }
        else if ( Input >= 104) {
          digitalWrite(RELAY_PIN_2, HIGH);
          RELAY_STATE_2 = 3;
        }
        else if (Input <= (Steamtemp -1)) {
          digitalWrite(RELAY_PIN_2, HIGH);
          RELAY_STATE_2 = 3;   
        }
      }
      else if (Input <= 96) {
        digitalWrite(RELAY_PIN_2, HIGH);
        RELAY_STATE_2 = 3;
      }
    }
    else {
      digitalWrite(RELAY_PIN_2, HIGH);
      RELAY_STATE_2 = 4;
    }
    lampTime = millis();
  }
    
  Status = 0;
  delay(980);
}

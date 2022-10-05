#include "Encoder.h"
#include <DualMC33926MotorShield.h>

DualMC33926MotorShield md;

#define PIN_A 2
#define RESET_PIN 3
#define PIN_B 5
#define TARGET_DELAY 100

// number of encoder counts in 90 degrees
#define QUARTER_TURN 800

Encoder encoder(PIN_A, PIN_B);
void reset();
void serialEvent();
void enableMotor(int pwr);

long lastSend = 0;          // Time in milliseconds that data was last sent to the PI
int encoderCounts = 0;      // Stored value of the encoder
int targetPosition = 0;     // Desired encoder value. Initially read in as a number 4 or less then converted to encoder counts
int convtargetPosition = 0; // Target position converted to encoder counts
int targetSet = 0;          // Saves target position motor is currently going to.
bool runMotor;              // Enable motor movement
int newCount = 0;
bool dataRead = true;

//Controller Code
double kp = 4.1156;
double ki = 0.37071;
double I = 0;
int e_past = 0;
int Ts = 0;
int Tc = 0;
double e = 0;
double r = 0;
double y = 0;


void setup() {
  Serial.begin(115200);
  runMotor = false;
  pinMode(RESET_PIN, INPUT);
  md.init();
  //attachInterrupt(digitalPinToInterrupt(RESET_PIN), reset, FALLING);
  lastSend = millis();
}


void loop() {
  newCount = encoder.read();

  //Controller Code for Loop
  int u = 0;
  y = (newCount - encoderCounts)/(millis()-lastSend);
  r = 0.1725;
  e = (r-y);
  I = I + Ts*e;
  u = (kp*e + (ki*I))*289.85;
  Ts = millis()-Tc;
  Tc = millis();
  
  //If Motor is enabled to run check to see where its value is into relation of target. If greater than or equal turn off motor.
  if(runMotor == true){
    if((newCount%3300) >= convtargetPosition){
        enableMotor(0);
        runMotor = false;
        dataRead = true;
    }
    else{
      if (u > 30){
        enableMotor(u);
      }
    }
  }
  // if newCount goes above encoder count for one full rotation reset to 0
  if(newCount >= 3300){
    encoder.write(0);
    newCount = encoder.read();
  }
        
    // Send the current possition to the pi
    // The raw count data is transmitted to the PI
    // 3200 counts per rotation => 1600 counts = 3.14
    // to get from counts to radians divide counts by 1590 then multiply by 3.14
  if (((millis() - lastSend) > TARGET_DELAY)) {
    Serial.println(newCount);
    Serial.flush();
    encoderCounts = newCount;
    lastSend = millis();
  }
}

void reset(){
  encoder.write(0);
  encoderCounts = 0;
}

void enableMotor(int pwr){
  md.setM1Speed(pwr);
}

void serialEvent() {
  if (dataRead == true){
    if(Serial.available() > 0) {
      String targetPositionStr = Serial.readStringUntil('\n');
      targetPosition = targetPositionStr.toInt();
    }
  //Only need to run below anytime the serialEvent 
    if((targetPosition != 0) && (targetSet != targetPosition)) {
      convtargetPosition = targetPosition * QUARTER_TURN;
      targetSet = targetPosition;
      runMotor = true;
      dataRead = false;
      enableMotor(50);
    }
  }
}

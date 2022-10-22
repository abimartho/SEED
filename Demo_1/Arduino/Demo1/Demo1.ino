#include <Encoder.h>
#include <DualMC33926MotorShield.h>
#include "conversions.h"
#include "control.h"

#define LEFT_PIN_A  2
#define RIGHT_PIN_A 3
#define LEFT_PIN_B  5
#define RIGHT_PIN_B 6

#define SAMPLE_TIME 10


Encoder masterWheel(LEFT_PIN_A, LEFT_PIN_B);
Encoder slaveWheel(RIGHT_PIN_A, RIGHT_PIN_B);

DualMC33926MotorShield md;

typedef enum {TEST2, TEST3SPIN, TEST3GO} TEST;
// Use to change which test is currently being performed
TEST currentTest = TEST3SPIN;
int lastTime = 0;

// Target values
int distanceTarget;
int targetMotorCounts;
double rotationTarget;

double theta = (3.05)*0.5*PI;
double thetaNow = 0.0;

// Encoder counts
int currentCountsMaster = 0;
int currentCountsSlave = 0;

// Error values
double distanceError = 0.0;
double wheelError = 0.0;
double angleError = 0.0;
double thetaError = 0.0;

// Array of mtrCtrlOut values, index 0 is master, index 1 is slave
int mtrVal[2] = {0, 0};

void setup() {
// Change the assigned values to change the distances covered by the robot
  distanceTarget = 12;
  distanceTarget = feetToCounts(distanceTarget);
  rotationTarget = PI;
  md.init();
  Serial.begin(9600);
}

void loop() {
  targetMotorCounts = inchesToCounts(distanceTarget);
  currentCountsMaster = masterWheel.read();
  currentCountsSlave = slaveWheel.read();
  wheelError = currentCountsMaster - currentCountsSlave;
  distanceError = distanceTarget - currentCountsMaster;
  angleError = currentCountsMaster - abs(currentCountsSlave);
  thetaNow = 5.875 * ((PI / 1600) * (currentCountsMaster - currentCountsSlave)) / 11;
  
  
  // Convert Errors to radians
  distanceError = distanceError * (PI / 1600);
  wheelError = wheelError * (PI / 1600);
  
  switch(currentTest){
    case TEST2:
      if(millis() >= SAMPLE_TIME + lastTime) {
        lastTime = millis();
        drive(distanceError, wheelError, mtrVal);
        md.setSpeeds(mtrVal[0], mtrVal[1]);
      }
      break;
      
    case TEST3SPIN:
      // TODO: Run motor to reach target angle
      if(millis() >= SAMPLE_TIME + lastTime) {
        lastTime = millis();
        turn(distanceError, angleError, mtrVal);
        md.setSpeeds(mtrVal[0], mtrVal[1]);
      }
      // TODO: transition to go state, reset encoders to 0
      thetaError = theta - thetaNow;
      if (thetaError <= 0) {
        masterWheel.write(0);
        slaveWheel.write(0);
        currentTest = TEST3GO;
      }
      break;

    case TEST3GO:
      if(millis() >= SAMPLE_TIME + lastTime) {
        lastTime = millis();
        drive(distanceError, wheelError, mtrVal);
        md.setSpeeds(mtrVal[0], mtrVal[1]);
      }
      break;
  }
}

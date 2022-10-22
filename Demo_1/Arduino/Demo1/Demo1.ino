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

typedef enum {TEST2, TEST3} TEST;
// Use to change which test is currently being performed
TEST currentTest = TEST2;
int lastTime = 0;

// Target distance in inches
int distanceTarget;
int targetMotorCounts;
// Target rotation in radians
double rotationTarget;

int currentCounts = 0;
// distance from target to be passed to motor controllers
int distanceError = 0;
// Difference between wheel values
int wheelError = 0;

// Array of mtrCtrlOut values, index 0 is master, index 1 is slave
int mtrVal[2] = {0, 0};

void setup() {
// Change the assigned values to change the distances covered by the robot
  distanceTarget = 12;
  distanceTarget = feetToCounts(distanceTarget);
  rotationTarget = 0;
  md.init();
  Serial.begin(9600);
}

void loop() {
  switch(currentTest){
    case TEST2:
      targetMotorCounts = inchesToCounts(distanceTarget);
      currentCounts = masterWheel.read();
      wheelError = currentCounts - slaveWheel.read();
      distanceError = distanceTarget - currentCounts;

      // Convert Errors to radians
      distanceError = distanceError * (PI / 1600);
      wheelError = wheelError * (PI / 1600);
      //Serial.print(distanceError);
      //Serial.print("\t");
      //Serial.println(wheelError);
      // TODO: add code to run the rightMotorCounts
      if(millis() >= SAMPLE_TIME + lastTime) {
        lastTime = millis();
        drive(distanceError, wheelError, mtrVal);
        Serial.print(mtrVal[0]);
        Serial.print("\t");
        Serial.println(mtrVal[1]);
        md.setSpeeds(mtrVal[0], mtrVal[1]);
      }
      break;
      
    case TEST3:
    
      // TODO: Run motor to reach target distance
      break;
  }
}

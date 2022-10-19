#include <Encoder.h>
#include <DualMC33926MotorShield.h>
#include "conversions.h"
#include "control.h"

#define LEFT_PIN_A  2
#define RIGHT_PIN_A 3
#define LEFT_PIN_B  4
#define RIGHT_PIN_B 5

Encoder leftWheel(LEFT_PIN_A, LEFT_PIN_B);
Encoder rightWheel(RIGHT_PIN_A, RIGHT_PIN_B);

DualMC33926MotorShield rightMotor;
DualMC33926MotorShield leftMotor;

typedef enum {TEST2, TEST3} TEST;
// Use to change which test is currently being performed
TEST currentTest = TEST2;

// Target distance in inches
int distanceTarget;
// Target rotation in radians
double rotationTarget;
// Targer postions to be used by the motor controllers
int rightMotorCounts = 0;
int leftMotorCounts = 0;
// distance from target to be passed to motor controllers
int rightError = 0;
int leftError = 0;

void setup() {
// Change the assigned values to change the distances covered by the robot
  distnaceTarget = 0;
  rotationTarget = 0;
}

void loop() {
  switch(currentTest){
    case TEST2:
      rightMotorCounts = inchesToCounts(distanceTarget);
      leftMotorCounts = rightMotorCounts
      // TODO: add code to run the rightMotorCounts
      rightMotor.setM1Speed(mtrValue(rightMotorCounts - rightMotor.read()));
      leftMotor.setM1Speed(mtrValue(leftMotorCounts - leftMotor.read()));
      break;
    case TEST3:
      // Rotate robot
      rightMotorCounts = radsToCounts(rotationTarget);
      // Assuming the motors behave identically
      leftMotorCounts = rightMotorCounts * -1
      // TODO: Run motor to rotate robot
      rightMotor.setM1Speed(mtrValue(rightMotorCounts - rightMotor.read()));
      leftMotor.setM1Speed(mtrValue(leftMotorCounts - leftMotor.read()));

      // Move forward
      rightMotorCounts = inchesToCounts(distanceTarget);
      leftMotorCounts = rightMotorCounts;
      // TODO: Run motor to reach target distance
      rightMotor.setM1Speed(mtrValue(rightMotorCounts - rightMotor.read()));
      leftMotor.setM1Speed(mtrValue(leftMotorCounts - leftMotor.read()));
      break;
  }
}

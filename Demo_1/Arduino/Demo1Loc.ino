#include "Encoder.h"
#include "conversions.h"

#define LEFT_PIN_A  2
#define RIGHT_PIN_A 3
#define LEFT_PIN_B  4
#define RIGHT_PIN_B 5

Encoder leftWheel(LEFT_PIN_A, LEFT_PIN_B);
Encoder rightWheel(RIGHT_PIN_A, RIGHT_PIN_B);

typedef enum {TEST2, TEST3} TEST;
TEST currentTest = TEST2;
// Target distance in inches
int distanceTarget;
// Target rotation in radians
double rotationTarget;
int motor1Counts = 0;
int motor2Coutns = 0;

void setup() {
// Change the assigned values to change the distances covered by the robot
distnaceTarget = 0;
rotationTarget = 0;
}

void loop() {
  switch(currentTest){
    case TEST2:
      motor1Counts = inchesToCounts(distanceTarget);
      motor2Counts = motor1Counts
      // TODO: add code to run the motor1Counts

      break;
    case TEST3:
      motor1Counts = radsToCounts(rotationTarget);
      // Assuming the motors behave identically
      motor2Counts = motor1Counts * -1
      // TODO: Run motor to
      motor1Counts = inchesToCounts(distanceTarget);
      motor2Counts = motor1Counts;
      // TODO: Run motor to reach target distance
      break;
  }
}

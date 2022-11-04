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

typedef enum {TEST2, TEST3SPIN,Test3wait, TEST3GO, SPIN} TEST;
// Use to change which test is currently being performed
TEST currentTest = SPIN; //TEST2//TEST3SPIN //SPIN
int lastTime = 0;

// Target values
double distanceTarget;
int targetMotorCounts;
//double rotationTarget;

double theta = -1.9*PI*(.5); //1.9 is fudge
double thetaNow = 0.0;

// Encoder counts
int currentCountsMaster = 0;
int currentCountsSlave = 0;

// Error values
double distanceError = 0.0;
double wheelError = 0.0;
double angleError = 0.0;
double thetaError = 0.0;

int dir=-1;
int testX=0;

// Array of mtrCtrlOut values, index 0 is master, index 1 is slave
int mtrVal[2] = {0, 0};

void setup() {
// Change the assigned values to change the distances covered by the robot
  distanceTarget = 1;
  distanceTarget = feetToCounts(distanceTarget);
  md.init();
  Serial.begin(115200);
}

void loop() {
  targetMotorCounts = inchesToCounts(distanceTarget);
  currentCountsMaster = masterWheel.read();
  currentCountsSlave = slaveWheel.read();
  wheelError = currentCountsMaster - currentCountsSlave;//bascially angle Error
  distanceError = distanceTarget - currentCountsMaster;
  angleError = currentCountsMaster - currentCountsSlave;//got rid of abs I think this might have messed it up.
  
  
  
  // Convert Errors to radians
  distanceError = distanceError * (PI / 1600);
  wheelError = wheelError * (PI / 1600);
  angleError=angleError*(PI/1600);//added
  
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
      if (theta>0){
        dir=1;
      }else{
        dir=-1;
      }
      thetaNow = 5.875 * ((PI / 1600) * (currentCountsMaster - currentCountsSlave)) / 11;
      if(millis() >= SAMPLE_TIME + lastTime) {
        lastTime = millis();  
        turn(dir, distanceError, angleError, mtrVal);
        //md.setSpeeds(mtrVal[0], mtrVal[1]);
      
      thetaError = abs(theta) - abs(thetaNow);
      //after doing some math this should work for both
      //-5--1=-4, 5-1=4 but we don't want negative for next if statement
      
      if (thetaError <= 0) {
        masterWheel.write(0);
        slaveWheel.write(0);
        currentTest = Test3wait;
      }else{
        md.setSpeeds(mtrVal[0],mtrVal[1]);
      }
      
      }
      // TODO: transition to go state, reset encoders to 0
      
      break;
      
    case Test3wait:
    md.setSpeeds(0,0);
    delay(1000);
    currentTest=TEST2;
    break;
    case TEST3GO:
    masterWheel.write(0);
    slaveWheel.write(0);
      if(millis() >= SAMPLE_TIME + lastTime) {
        lastTime = millis();
        drive(distanceError, wheelError, mtrVal);
        md.setSpeeds(mtrVal[0], mtrVal[1]);
      }
      break;
    
    case SPIN:
    delay(100);
    if(testX<50){ //this just needs to be the intterupt
    md.setSpeeds(150,150);
    testX++;
    }else{
      md.setSpeeds(0,0);
      currentTest= TEST3SPIN;
      masterWheel.write(0);
      slaveWheel.write(0);
      delay(1000);
    }
  
    break;
    }
 
}

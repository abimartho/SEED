#include <Encoder.h>
#include <DualMC33926MotorShield.h>
#include "conversions.h"
#include "control.h"
#include <Wire.h>

// Encoder Pin Definitions and Instantiation
#define LEFT_PIN_A  2
#define LEFT_PIN_B  5

#define RIGHT_PIN_A 3
#define RIGHT_PIN_B 6

Encoder masterWheel(LEFT_PIN_A, LEFT_PIN_B);
Encoder slaveWheel(RIGHT_PIN_A, RIGHT_PIN_B);

// Sample Delay Time
#define SAMPLE_TIME 10

DualMC33926MotorShield md;

// Arduino FSM States
typedef enum {START, SEARCH, DELAY4STOP, RECEIVECMD, WAIT, TURN, DRIVE, STOP} MODE;

// Use to change which test is currently being performed
MODE currentMode = START; 
int lastTime = 0;

// I2C Variables
#define SLAVE_ADDRESS 0x04
int offsetReg = 0;        //Current 
int data[8];          //I2C "Blocks"
int startReg = 0;
int stopReg = 1;
int dimensionsReg = 2;
int readReg = 4;      //ReadReg all I2C coms starting with 4 are a read
int readStatus = 0;   //Updates based on Arduino's State to Tell Pi

// Target values
double distanceTarget = 0.0;
double angleTarget = 0.0;
double distanceReceived = 0.0;
double angleReceived = 0.0;
int targetMotorCounts = 0;
//double rotationTarget;

double theta = -1.9*PI*(.5); //1.9 is fudge
double thetaNow = 0.0;

// Encoder counts
double currentCountsMaster = 0.0;
double currentCountsSlave = 0.0;

// Error values
double distanceError = 0.00;//how far away am I
double rotationError = 0.00;//what angle am I facing
double wheelError = 0.00;//error between wheels
double angleError = 0.00;//error between wheels
double thetaError = 0.00;//what angle am I facing

// General Command/Tracking
int markerCount = 1;
int stopCMD = 0;
int startCMD = 1;

// Array of mtrCtrlOut values, index 0 is master, index 1 is slave
int mtrVal[2] = {0, 0};

void setup() {
  pinMode(13, OUTPUT);
  // Change the assigned values to change the distances covered by the robot
  // distanceTarget = 1;
  // distanceTarget = feetToCounts(distanceTarget);
  md.init();
  Serial.begin(115200);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}

void loop() {
  targetMotorCounts = inchesToCounts(distanceTarget);
  currentCountsMaster = masterWheel.read();//get encoder values
  currentCountsSlave = slaveWheel.read();
  wheelError = currentCountsMaster - currentCountsSlave;//bascially angle Error
  distanceError = distanceTarget - currentCountsMaster;

  
  
  // Convert Errors to radians
  distanceError = distanceError * (PI / 1600);
  wheelError = wheelError * (PI / 1600); 

  
  switch(currentMode){
    case START:
      if (startCMD == 1){
        currentMode = SEARCH;
        startCMD = 0;
      }
    break;

    case DRIVE:
      //if(millis() >= SAMPLE_TIME + lastTime) {
        //lastTime = millis();
        drive(distanceError, wheelError, mtrVal);
        md.setSpeeds(mtrVal[0], mtrVal[1]);
        if(distnaceError < 100) {
          currentMode = STOP;
        }
      //}
      break;
      
    case TURN:
      // TODO: Run motor to reach target angle
      rotationError = angleTarget - currentCountsMaster;
      angleError = currentCountsMaster - currentCountsSlave;
      angleError=angleError*(PI/1600);//added
      if (angleReceived > 0.0){
        dir=1;
      }else{
        dir=-1;
      }
      thetaNow = 5.875 * ((PI / 1600) * (currentCountsMaster - currentCountsSlave)) / 11;
      if(millis() >= SAMPLE_TIME + lastTime) {
        lastTime = millis();  
        turn(dir, rotationError, angleError, mtrVal);
        thetaError = abs(angleTarget) - abs(thetaNow);//make sure it is the angle we want it
     
      
        if (thetaError <= 0.0) {
          currentMode = WAIT;
        }else{
          md.setSpeeds(mtrVal[0],mtrVal[1]);
        }
      }
      // TODO: transition to go state, reset encoders to 0
      
      break;
      
    case WAIT:
      md.setSpeeds(0,0);
      masterWheel.write(0);
      slaveWheel.write(0);
      delay(1000);
      currentMode = DRIVE;
      readStatus = 2;
    break;

    case DELAY4STOP:
      readStatus = 5; // Tell PI it is waiting to receive dimensions
      if (offsetReg == 0){
        masterWheel.write(0);
        slaveWheel.write(0);
        currentMode = RECEIVECMD;
      }
    break;
    
    case RECEIVECMD:
      if(offsetReg == 2){
        angleTarget = radsToCounts(angleReceived);
        distanceTarget = feetToCounts(distanceReceived  - 0.2);
        currentMode = TURN;
      }
    break;

    case SEARCH:
      if(stopCMD == 1) {
        md.setSpeeds(0, 0);
        masterWheel.write(0);
        slaveWheel.write(0);
        currentMode = DELAY4STOP;
      } else if(millis() % 500 < 250 && stopCMD != 1) {
        md.setSpeeds(125, 125);
      } else if (millis() % 500 > 250) {
        md.setSpeeds(0, 0);
      }
    break;

    case STOP:
      if (markerCount != 6){
        readStatus = 6; // Tells Pi to go to next Marker
        delay(5000);
        markerCount += 1;
        currentMode = SEARCH;
      }
      else{
        readStatus = 7; // Tells Pi that we are done. (Keep in for now, to test CLYDE will want to be able to easily restart Clyde)
        currentMode = START; // Can switch to "Party" once we have working
      }
    }  
}


void receiveData(int byteCount){
  int i=0;
  while( Wire.available()) {
    data[i] = Wire.read();
    i++;
  }
  i--;
  if (data[0] == dimensionReg) {
    offsetReg = dimensionReg;
    if (data[2] > 127){
      data[2] = 256 - data[2]; // Python Byte is 2's Complement, Arduino does not natively recognize that.
      data[2] *= -1;
    }
    angleReceived = data[2]/(200.0);
    distanceReceived = data[3]/10.0;
  }
  else if(data[0] == startReg){
    startCMD = 1;
  }
  else if (data[0] == stopReg){
    stopCMD = 1;
    offsetReg = stopReg;
  }
  else if (data[0] == readReg){
    offsetReg = readReg;
  }
}

// callback for sending data
void sendData(){
  if (offsetReg == readReg){
    Wire.write(readStatus);
    offsetReg = 0;
  }
}

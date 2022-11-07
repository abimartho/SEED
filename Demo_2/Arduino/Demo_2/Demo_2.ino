#include <Encoder.h>
#include <DualMC33926MotorShield.h>
#include "conversions.h"
#include "control.h"
#include <Wire.h>


#define LEFT_PIN_A  2
#define RIGHT_PIN_A 3
#define LEFT_PIN_B  5
#define RIGHT_PIN_B 6

#define SAMPLE_TIME 10


Encoder masterWheel(LEFT_PIN_A, LEFT_PIN_B);
Encoder slaveWheel(RIGHT_PIN_A, RIGHT_PIN_B);

DualMC33926MotorShield md;

typedef enum {SEARCH, WAIT, TURN, DRIVE} MODE;
// Use to change which test is currently being performed
MODE currentMode = SEARCH; 
int lastTime = 0;

#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;
int data[8];
int readReg = 4;
int readStatus = 0;

// Target values
double distanceTarget = 0.0;
int targetMotorCounts = 0;
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
int stopCMD = 0;
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
  currentCountsMaster = masterWheel.read();
  currentCountsSlave = slaveWheel.read();
  wheelError = currentCountsMaster - currentCountsSlave;//bascially angle Error
  distanceError = distanceTarget - currentCountsMaster;
  angleError = currentCountsMaster - currentCountsSlave;//got rid of abs I think this might have messed it up.
  
  
  
  // Convert Errors to radians
  distanceError = distanceError * (PI / 1600);
  wheelError = wheelError * (PI / 1600);
  angleError=angleError*(PI/1600);//added
  
  switch(currentMode){
    case DRIVE:
      if(millis() >= SAMPLE_TIME + lastTime) {
        lastTime = millis();
        drive(distanceError, wheelError, mtrVal);
        md.setSpeeds(mtrVal[0], mtrVal[1]);
      }
      break;
      
    case TURN:
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
        currentMode = WAIT;
      }else{
        md.setSpeeds(mtrVal[0],mtrVal[1]);
      }
      
      }
      // TODO: transition to go state, reset encoders to 0
      
      break;
      
    case WAIT:
    md.setSpeeds(0,0);
    delay(1000);
    currentMode = DRIVE;
    break;
    
    /*case TEST3GO:
    masterWheel.write(0);
    slaveWheel.write(0);
      if(millis() >= SAMPLE_TIME + lastTime) {
        lastTime = millis();
        drive(distanceError, wheelError, mtrVal);
        md.setSpeeds(mtrVal[0], mtrVal[1]);
      }
      break;
    */
    case SEARCH:
      delay(100);
      if(stopCMD != 1){ //this just needs to be the intterupt
        md.setSpeeds(150,150);
        //testX++;
      }else{
        md.setSpeeds(0,0);
        currentMode = TURN;
        masterWheel.write(0);
        slaveWheel.write(0);
        delay(1000);
        targetAngle = angleReceived
        targetDistance = distanceReceived
      }
    break;
    }  
}

void receiveData(int byteCount){
  int i=0;
  while( Wire.available()) {
    data[i] = Wire.read();
    Serial.print(data[i]);
    Serial.print(' ');
    i++;
  }
  i--;
  //Arduino recieved angle stop motors and angle and distance
  if (data[0] == 0) {
    state = 0;
    //readStatus = 0;
    stopCMD = 1;
    angleReceived = data[1]/1000;
    distanceReceived = data[2]/10;
  }
  //Arduino has recieved Angle
  /*else if (data[0] == 1) {
    state = 1;
    angleReceived = data[1]/1000;
    readStatus = 1;
  }
  //Arduino has recieved Distance
  else if (data[0] == 2){
    state = 2;
    distanceReceived = data[1]/10;
    readStatus = 2;
  }*/
  //Read Request
  else if (data[0] == 4){
    state = 4;
  }
}

// callback for sending data
void sendData(){
  if (state == 4){
    Wire.write(readStatus);
  }
  /*else if (state == 0) {
    //Use to indicate that Arduino has stopped and wants Angle
    Wire.write(0);
  }
  else if(state == 1){
    Wire.write(1); //Indicate Arduino is Lined up With Angle
  }
  else if(state == 2){
    Wire.write(2);  //Indiacte Arduino is at Marker
  }*/
}

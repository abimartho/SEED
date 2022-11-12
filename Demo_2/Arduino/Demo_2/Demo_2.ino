#include <Encoder.h>
#include <DualMC33926MotorShield.h>
#include "conversions.h"
#include "control.h"
#include <Wire.h>


#define LEFT_PIN_A  2
#define LEFT_PIN_B  5

#define RIGHT_PIN_A 3
#define RIGHT_PIN_B 6

#define SAMPLE_TIME 10


Encoder masterWheel(LEFT_PIN_A, LEFT_PIN_B);
Encoder slaveWheel(RIGHT_PIN_A, RIGHT_PIN_B);

DualMC33926MotorShield md;

typedef enum {SEARCH, WAIT, TURN, DRIVE, WAITDIST} MODE;
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

int dir=-1; //which way
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
  currentCountsMaster = masterWheel.read();//get encoder values
  currentCountsSlave = slaveWheel.read();
  wheelError = currentCountsMaster - currentCountsSlave;//bascially angle Error
  distanceError = distanceTarget - currentCountsMaster;

  
  
  // Convert Errors to radians
  distanceError = distanceError * (PI / 1600);
  wheelError = wheelError * (PI / 1600); 

  
  switch(currentMode){
    case DRIVE:
      //if(millis() >= SAMPLE_TIME + lastTime) {
        //lastTime = millis();
        drive(distanceError, wheelError, mtrVal);
        md.setSpeeds(mtrVal[0], mtrVal[1]);
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

    case WAITDIST:
      if (state == 0){
        angleTarget = radsToCounts(angleReceived);
        distanceTarget = feetToCounts(distanceReceived  - 0.2);
        masterWheel.write(0);
        slaveWheel.write(0);
        currentMode = TURN;
      }
    break;
    
    case SEARCH:
      if(stopCMD == 1) {
        md.setSpeeds(0, 0);
       // delay(500);
        //angleTarget = radsToCounts(angleReceived);
        //distanceTarget = feetToCounts(distanceReceived  - 0.2);
        masterWheel.write(0);
        slaveWheel.write(0);
        currentMode = WAITDIST;
      } else if(millis() % 500 < 250 && stopCMD != 1) {
        md.setSpeeds(125, 125);
      } else if (millis() % 500 > 250) {
        md.setSpeeds(0, 0);
      }
    break;
    }  
}

void receiveData(int byteCount){
  int i=0;
  while( Wire.available()) {
    data[i] = Wire.read();
    Serial.println(data[i]);
    Serial.print(' ');
    i++;
  }
  i--;
  //Arduino recieved angle stop motors and angle and distance
  if (data[0] == 0) {
    state = 0;
    //readStatus = 0;
    //stopCMD = 1;
    Serial.println("Received Values");
    if (data[2] > 127){
      data[2] = 256 - data[2];
      data[2] *= -1;
    }
    angleReceived = data[2]/(200.0);
    distanceReceived = data[3]/10.0;
    Serial.println(angleReceived);
    Serial.println(distanceReceived);
  }
  else if(data[0] == 1){
    state = 1;
    readStatus = 1;
    stopCMD = 1;
  }
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
}

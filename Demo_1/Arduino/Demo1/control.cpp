#include "control.h"

// Takes the errors as an input, updates mtrVal array.
void drive(double targetError, double wheelError, int mtrVal[2]) {
  double scale = 0.0;
  double mtrCtrlOut = 0;
  double mtrCtrlOut2 = 0;
  double mtrCtrlVolt = 0;
  double mtrCtrlVolt2 = 0;
  
  mtrCtrlVolt = kp * targetError;
  mtrCtrlVolt2 = kp2 * wheelError + mtrCtrlVolt;

  mtrCtrlOut=(mtrCtrlVolt/8.0)*400; 
  mtrCtrlOut2=(mtrCtrlVolt2/8.0)*400;

  if (mtrCtrlOut<-400){ //max is 400 put to 300 to try and reduce slip
    scale = -400 / mtrCtrlOut;
    mtrCtrlOut = -400;
    mtrCtrlOut2 = scale * mtrCtrlOut2;
  }
  if (mtrCtrlOut>400){
    scale = 400 / mtrCtrlOut;
    mtrCtrlOut = 400;
    mtrCtrlOut2 = scale * mtrCtrlOut2;
  }
  if (mtrCtrlOut2 < -400) {
    scale = -400 / mtrCtrlOut2;
    mtrCtrlOut2 = -400;
    mtrCtrlOut = scale * mtrCtrlOut;
  }
  if (mtrCtrlOut2 > 400) {
    scale = 400 / mtrCtrlOut2;
    mtrCtrlOut2 = 400;
    mtrCtrlOut = scale * mtrCtrlOut;
  }
  mtrVal[0] = mtrCtrlOut;
  mtrVal[1] = -1 * mtrCtrlOut2;
}

void turn (double error, double error2, int mtrVal[2]) {
  double mtrCtrlOut = 0;
  double mtrCtrlOut2 = 0;
  double mtrCtrlVolt = 0;
  double mtrCtrlVolt2 = 0;
  
  mtrCtrlVolt=kp*error; //this is in volts
  mtrCtrlVolt2=(mtrCtrlVolt)+(kp2*error2); //We will need to tweek kp2 so that the motors line up
    
    
  mtrCtrlOut=((mtrCtrlVolt/7.5)*50); //300 was 400
  mtrCtrlOut2=(mtrCtrlVolt2/7.5)*50;
    
  if (mtrCtrlOut<-delta){ 
  mtrCtrlOut=-delta;
  }
  if (mtrCtrlOut>delta){
    mtrCtrlOut=delta;
  }
  if (mtrCtrlOut2<-delta){ 
    mtrCtrlOut2=-delta;
  }
  if (mtrCtrlOut2>delta){
    mtrCtrlOut2=delta;
  }

  mtrVal[0] = (mtrCtrlOut + mtrCtrlOut2) / 2;
  mtrVal[1] = (mtrCtrlOut + mtrCtrlOut2) / 2;
}

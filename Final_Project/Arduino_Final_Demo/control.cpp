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

  if (mtrCtrlOut < -400){ //max is 400 put to 300 to try and reduce slip
    scale = -400 / mtrCtrlOut;
    mtrCtrlOut = -400;
    mtrCtrlOut2 = scale * mtrCtrlOut2;
  }
  else if (mtrCtrlOut > 400){
    scale = 400 / mtrCtrlOut;
    mtrCtrlOut = 400;
    mtrCtrlOut2 = scale * mtrCtrlOut2;
  }
  if (mtrCtrlOut2 < -400) {
    scale = -400 / mtrCtrlOut2;
    mtrCtrlOut2 = -400;
    mtrCtrlOut = scale * mtrCtrlOut;
  }
  else if (mtrCtrlOut2 > 400) {
    scale = 400 / mtrCtrlOut2;
    mtrCtrlOut2 = 400;
    mtrCtrlOut = scale * mtrCtrlOut;
  }
  mtrVal[0] = mtrCtrlOut;
  mtrVal[1] = -1 * mtrCtrlOut2;
}

void turn (int dir, double error, double error2, int mtrVal[2]) {
  double mtrCtrlOut = 0;
  double mtrCtrlOut2 = 0;
  double mtrCtrlVolt = 0;
  double mtrCtrlVolt2 = 0;
  double spinOther=0;

  if (dir == 1){ // Clyde is making a Right turn
    mtrCtrlVolt=kp*error; //this is in volts
    mtrCtrlVolt2=(mtrCtrlVolt)+(kp2*error2)+152; //We will need to tweek kp2 so that the motors line up
    
    
    mtrCtrlOut = ((mtrCtrlVolt/7.5)*100); //was 50 but too weak
    mtrCtrlOut2 = ((mtrCtrlVolt2/7.5)*100);
    
    if(mtrCtrlOut < -delta){ 
      mtrCtrlOut = -delta;
    }
    else if(mtrCtrlOut > delta){
      mtrCtrlOut = delta;
    }
    if (mtrCtrlOut2 < -delta){ 
     mtrCtrlOut2 = -delta;
    }
    else if (mtrCtrlOut2 > delta){
      mtrCtrlOut2 = delta;
    }
    mtrVal[0]= mtrCtrlOut;
    mtrVal[1]= mtrCtrlOut2;
  
  } else{ // Clyde is making a Left turn
    //ok this works, but obviously isn't what we want
    //mtrVal[0]=-delta; //positive turns right
    //mtrVal[1]=-delta;
    mtrCtrlVolt=kp*error; //this is in volts
    mtrCtrlVolt2=(mtrCtrlVolt)+(kp2*error2)+152; //if this doesn't work mod 152
      
    mtrCtrlOut=((mtrCtrlVolt/7.5)*100); //was 50 but too weak
    mtrCtrlOut2=((mtrCtrlVolt2/7.5)*100);

    if (mtrCtrlOut < delta){ //-delta and delta
      mtrCtrlOut= delta;
    }
    else if (mtrCtrlOut > -delta){
      mtrCtrlOut= -delta;
    }
    if (mtrCtrlOut2 < delta){
      mtrCtrlOut2 = delta;
    }
    else if (mtrCtrlOut2 > -delta){
      mtrCtrlOut2 = -delta;
    }
    
    mtrVal[0]=-mtrCtrlOut;
    mtrVal[1]=-mtrCtrlOut2;
  
  }

}

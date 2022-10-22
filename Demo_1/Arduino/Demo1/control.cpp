#include "control.h"

// Takes the errors as an input, updates mtrVal array.
void drive(double targetError, double wheelError, int mtrVal[2]) {
  double scale = 0.0;
  double mtrCtrlOut = 0;
  double mtrCtrlOut2 = 0;
  double mtrCtrlVolt = 0;
  double mtrCtrlVolt2 = 0;
  
  mtrCtrlVolt = kp * targetError;
  Serial.println(mtrCtrlVolt);
  mtrCtrlVolt2 = kp2 * wheelError + mtrCtrlVolt;
  Serial.println(mtrCtrlVolt2);

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
  Serial.println(mtrCtrlOut);
  mtrVal[0] = mtrCtrlOut;
  Serial.println(mtrCtrlOut2);
  mtrVal[1] = -1 * mtrCtrlOut2;
}

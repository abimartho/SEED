#include "control.h"

// Takes the error as an input, returns value to write to motor.
int mtrValue(double error) {
  int mtrCtrl = 0;
  int integral = 0;

  if (error < 5) { //no idea what this value should be I will figure it out //if it is jerky too much, else too little
    integral = integral + (error * CONTROL_SAMPLE_TIME) / 100;
  }
  else {
    integral = 0;
  }

  mtrCtrl = KP * error + KI * integral;

  if(mtrCtrl > 400) {
    mtrCtrl = 400;
  }
  else if(mtrCtrl < -400) {
    mtrCtrl = -400;
  }
  return mtrCtrl;
}

#include <Encoder.h>
#include <DualMC33926MotorShield.h>

#define ENC_A 2
#define ENC_B 5
#define RESET_PIN 3 //I beleive unessisary

#define ENC_C 3 //are these numbers good
#define ENC_D 6

Encoder myEnc(ENC_A, ENC_B);
Encoder myEnc2(ENC_C,ENC_D);
DualMC33926MotorShield md;

double scale = 0.0;

int cir=PI*5.875;//change 5
double distance=150;//Put in inches if you are measuring wheel radius in inch

double angleWant=2*PI*(distance)/cir; //double check my math on this
//double angleWant=2*3.14; //1 rotation for testing

//variables for first encoder
double angleNow=0;
double error;
double kp = 8;//og 4.1156
double ki = 0; //og .37071
int sampTime=10; //this should be constant 10 ms
int nowTime=0; //this should be current time
int integral=0; //this will be the case until it is very small
int mtrCtrlVolt;
float mtrCtrlOut;

//variables for the second encoder
double angleNow2=0;
double error2=0;
double mtrCtrlVolt2;
float mtrCtrlOut2;
double kp2=8;

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);
  //Serial.println("Dual MC33926 Motor Shield");
  md.init();
}

void loop() {
  // I need to implement this to run every 10 ms(Time1)
  if(millis() >= sampTime + nowTime);{
    nowTime+=sampTime;
  angleNow=(myEnc.read())*(PI/1600);//gets angle in radians
  angleNow2=(myEnc2.read())*(PI/1600);
  //Serial.println("1");
  //Serial.println(myEnc.read());
  //Serial.println("2");
  //Serial.println(myEnc2.read());
   // Serial.println("Motor 1"); //do I want print or println
   // Serial.println(angleNow);
    //Serial.println("Motor 2");
    //Serial.println(angleNow2);
    error=angleWant-angleNow;
    error2=angleNow-angleNow2;
    Serial.println("Error 1");
   // Serial.println(error);
    Serial.println("Error 2");
    //Serial.println(error2);
      
      //don't need kd
      //kp and ki figure out through trial and error speed=kp jitters=ki
    mtrCtrlVolt=kp*error+ki*integral; //this is in volts
   
    mtrCtrlVolt2=kp2*error2+mtrCtrlVolt; //We will need to tweek kp2 so that the motors line up
   
   
    mtrCtrlOut=(mtrCtrlVolt/8.0)*400; //300 was 400
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
   // mtrCtrlOut2 = mtrCtrlOut;
  }
  md.setSpeeds(mtrCtrlOut,-1*mtrCtrlOut2);
 // md.setM2Speed(mtrCtrlOut2); //this was individual 
}

//if motor isn't moving jack up kp until it does.

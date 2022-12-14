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

double delta=100;

int cir=PI*5.875;//change 5
double distance=100;//Put in inches if you are measuring wheel radius in inch

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

double theta=(3.05)*.5*PI;
double thetaNow=0;
double error3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Serial.println("Dual MC33926 Motor Shield");
  md.init();
  pinMode(RESET_PIN, INPUT); //do i need this?
}

void loop() {
  // I need to implement this to run every 10 ms(Time1)
  if(millis() >= sampTime + nowTime);{
    nowTime+=sampTime;
  angleNow=(myEnc.read())*(PI/1590);//gets angle in radians
  angleNow2=(myEnc2.read())*(PI/1590);
  thetaNow=5.875*(angleNow-angleNow2)/11;
  Serial.println("1");
  Serial.println(myEnc.read());
  Serial.println("2");
  Serial.println(myEnc2.read());
   // Serial.println("Motor 1"); //do I want print or println
   // Serial.println(angleNow);
    //Serial.println("Motor 2");
    //Serial.println(angleNow2);
    error=angleWant-angleNow;
    error2=angleNow-abs(angleNow2);
    error3=theta-thetaNow;
    //Serial.println("Error 1");
   // Serial.println(error);
    //Serial.println("Error 2");
    //Serial.println(error2);
  Serial.println("ERROR 3");
  Serial.println(error3);
    mtrCtrlVolt=kp*error; //this is in volts
    mtrCtrlVolt2=(mtrCtrlVolt)+(kp2*error2); //We will need to tweek kp2 so that the motors line up
    
    
    mtrCtrlOut=((mtrCtrlVolt/7.5)*50); //300 was 400
    mtrCtrlOut2=(mtrCtrlVolt2/7.5)*50;
    
    if (mtrCtrlOut<-delta){ //max is 400 put to 300 to try and reduce slip
    mtrCtrlOut=-delta;
    }
    if (mtrCtrlOut>delta){
      mtrCtrlOut=delta;
    }
    if (mtrCtrlOut2<-delta){ 
      mtrCtrlOut=-delta;
    }
    if (mtrCtrlOut2>delta){
      mtrCtrlOut=delta;
    }
    
  }
  if(error3>0){
  md.setSpeeds((mtrCtrlOut+mtrCtrlOut2)/2,(mtrCtrlOut+mtrCtrlOut2)/2);
  //md.setSpeeds((mtrCtrlOut),(mtrCtrlOut2));//md.setSpeeds((mtrCtrlOut)-1*mtrCtrlOut2);
  }
  if(error3<=0){
    md.setSpeeds(0,0);
  }
}

//if motor isn't moving jack up kp until it does.

// TODO Arduino.h is just for using CLion, you can remove it in yours
#include <Encoder.h>
#include <DualMC33926MotorShield.h>

#define ENC_A 3
#define ENC_B 5
#define RESET_PIN 3

Encoder myEnc(ENC_A, ENC_B);
DualMC33926MotorShield md;

double angleWant = 2 * 3.14; //1 rotation for testing
// TODO PI is a constant so you can use PI instead of 3.14

double angleNow = 0;
double error;

const double KP = 4.1156;
const double KI = 0.37071;

int CONTROL_SAMPLE_TIME = 10; //this should be constant 10 ms
int currentTime = 0; //this should be current time

int integral = 0; //this will be the case until it is very small
int mtrCtrl = KP * error + KI * integral;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Dual MC33926 Motor Shield");
  md.init();
  pinMode(RESET_PIN, INPUT); //do i need this?
  // I dont think so
  Serial.print("Angle Wanted: ");//setup
  Serial.println(angleWant);
}

void loop() {
  // I need to implement this to run every 10 ms(Time1)
  if (millis() >= currentTime + CONTROL_SAMPLE_TIME) {

    // TODO this
    currentTime += CONTROL_SAMPLE_TIME;
    // is the same as
    // currentTime = currentTime + CONTROL_SAMPLE_TIME
    angleNow = (myEnc.read()) * (3.14 / 1590); //gets angle in radians

    Serial.println(angleNow);
    error = angleWant - angleNow;
    Serial.println(error);

    if (error < 5) { //no idea what this value should be I will figure it out //if it is jerky too much, else too little
      integral = integral + (error * CONTROL_SAMPLE_TIME) / 100;
    }
    else {
      integral = 0;
    }
    //KP and KI were simulated and don't need kd
    mtrCtrl = KP * error + KI * integral;

    if (mtrCtrl < 400 & mtrCtrl > 0) { //I based max speed on experiment and found 11.198 rad/sec is max speed
      //what do I put in here
    }
    
    // TODO limit the maximum output directly, like this:
    if(mtrCtrl > 400) {
      mtrCtrl = 400;
    }
    else if(mtrCtrl < -400) {
      mtrCtrl = -400;
    }
  }
  // Setting the motor speed should be outside the sample time if statement
  md.setM1Speed(mtrCtrl);
}

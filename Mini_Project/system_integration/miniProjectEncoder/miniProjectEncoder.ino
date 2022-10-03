#include "Encoder.h"
#include <DualMC33926MotorShield.h>

DualMC33926MotorShield md;
//#define ENC_A 3
#define ENC_B 5
//Encoder encoder(ENC_A, ENC_B);

long angleV[401]={0};
double velocity=0;



#define PIN_A 2
#define RESET_PIN 3
#define PIN_B 5
#define TARGET_DELAY 100

Encoder encoder(PIN_A, PIN_B);
void reset();
void serialEvent();

long lastSend = 0;
int encoderCounts = 0;
int targetPosition;

int period = 5;
unsigned long time_now = 0;

int angle=5;
//l angleV[201]={0};
//int velocity=0;
int a=1;
int hundCount=0;

int encoderLoop = 3170;
int negEncoderLoop = -3170;

bool fullRotate = true;
bool simRotate = false;

void setup() {
  Serial.begin(115200);

  pinMode(RESET_PIN, INPUT);
  md.init();
  attachInterrupt(digitalPinToInterrupt(RESET_PIN), reset, FALLING);
  lastSend = millis();
  
  //Slowly ramp up speed of motor to reduce chance of burnout from putting in 400 motor current.
  if (fullRotate == true){
    //TARGET_DELAY = 100;
    for (int i = 1; i <= 100; i++){
      md.setM1Speed(i);
      //stopIfFault();
    }
  }
  else if (simRotate == true){
    //TARGET_DELAY = 1;
    for (int i = 1; i <= 50; i++){
      md.setM1Speed(i);
    }
  }
  else{
    //TARGET_DELAY = 100;
    for (int i = 1; i <= 400; i++){
      md.setM1Speed(i);
    }
  }
}


void loop() {
  // Read in the current possition
  int newCount = encoder.read();

  /* Send the current possition to the pi
  // The raw count data is transmitted to the PI
  // ~3180 counts per rotation => 1590 counts = 3.14
  // to get from counts to radians divide counts by 1590 then multiply by 3.14
  // angularPosition = (encoderCounts / 40.0) * 3.14;*/
  if (((millis() - lastSend) > TARGET_DELAY)) {
    if (newCount >= targetPosition or newCount <= (-1 * targetPosition)){
      //encoder.write(0);
      //newCount = encoder.read();
      Serial.println(newCount);
      for(int i = 100; i >= 0; i--){
        md.setM1Speed(i); 
      }
    }
    else if (simRotate == true and (newCount >= encoderLoop/6 or newCount <= (negEncoderLoop/6))){
      Serial.println(newCount);
      /*for(int i = 50; i >= 0; i--){
        md.setM1Speed(i); 
      }*/
    }
    else if ((newCount >= encoderLoop or newCount <= (negEncoderLoop))){
      encoder.write(0);
      newCount = encoder.read();
      Serial.println(newCount);
    }
    else{
      Serial.println(newCount);
    }
    encoderCounts = newCount;
    lastSend = millis();
  }
}

void reset() {
  encoderCounts = 0;
  encoder.write(0);
}

void serialEvent() {
  if(Serial.available() > 0) {
    int targetPositionStr = Serial.read();
    targetPosition = (targetPositionStr* 1500)/3.14;
  }
  Serial.flush();
}

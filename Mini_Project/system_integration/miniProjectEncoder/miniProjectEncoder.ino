#include "Encoder.h"
#include <DualMC33926MotorShield.h>

//==================================================================================================
//                                             DECLARATIONS
//==================================================================================================

DualMC33926MotorShield md;

#define PIN_A 2
#define RESET_PIN 3
#define PIN_B 5
#define TARGET_DELAY 100

long angleV[401]={0};
double velocity=0;

Encoder encoder(PIN_A, PIN_B);

long lastSend = 0;
int encoderCounts = 0;
int targetPosition = 0;

int period = 5;
unsigned long time_now = 0;

int angle=5;
//l angleV[201]={0};
//int velocity=0;
int a=1;
int hundCount=0;

int encoderLoop = 3170;

bool fullRotate = true;
// toggle for controls simultion
bool simRotate = false;

// FUNCTION STUBS
void reset();
void serialEvent();

//==================================================================================================
//                                              SETUP
//==================================================================================================

void setup() {
  // open serial port at baud rate 115200
  Serial.begin(115200);

  pinMode(RESET_PIN, INPUT);
  md.init();
  attachInterrupt(digitalPinToInterrupt(RESET_PIN), reset, FALLING);
  lastSend = millis();
  // speed init for main functioning
  if (fullRotate = true){
    //TARGET_DELAY = 100;
    for (int i = 1; i <= 100; i++){
      md.setM1Speed(i);
      //stopIfFault();
    }
  }
  // speed init for controls simulation
  else if (simRotate = true){
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

//==================================================================================================
//                                              LOOP
//==================================================================================================

void loop() {
  //Slowly ramp up speed of motor to reduce chance of burnout from putting in 400 motor current.
  // Read in the current position
  int newCount = encoder.read();

  // Send the current position to the pi as raw count
  // ~3180 counts per rotation => 1590 counts = 3.14
  // to get from counts to radians divide counts by 1590 then multiply by 3.14
  // angularPosition = (encoderCounts / 40.0) * 3.14;
  if (((millis() - lastSend) > TARGET_DELAY)) { // purpose of this conditional?
    // conditional for rotation to specified point
    if ((newCount >= encoderLoop or newCount <= (-1*encoderLoop)) and fullRotate == true){
      encoder.write(0);
      newCount = encoder.read();
      Serial.println(newCount);
      for(int i = 100; i >= 0; i--){
        md.setM1Speed(i); 
      }
    }
    // conditional for controls simulation
    else if (simRotate = true and (newCount >= encoderLoop/6 or newCount <= (-1*encoderLoop/6))){
      for(int i = 100; i >= 0; i--){
        md.setM1Speed(i); 
      }
    }
    else{
      Serial.println(newCount);
    }
    encoderCounts = newCount;
    lastSend = millis();
  }
}

//==================================================================================================
//                                     FUNCTION DEFINITIONS
//==================================================================================================

void reset() {
  encoderCounts = 0;
  encoder.write(0);
}

void serialEvent() {
  if(Serial.available() > 0) {
    String targetPositionStr = Serial.readStringUntil('\n');
    targetPosition = targetPositionStr.toInt();
  }
  Serial.flush();
}

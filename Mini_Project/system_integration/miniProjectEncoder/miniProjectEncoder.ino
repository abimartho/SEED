#include "Encoder.h"

#define PIN_A 2
#define RESET_PIN 3
#define PIN_B 5
#define TARGET_DELAY 100

Encoder encoder(PIN_A, PIN_B);
void reset();
void serialEvent();

long lastSend = 0;
int encoderCounts = 0;
int targetPosition = 0;

int period = 5;
unsigned long time_now = 0;

int angle=5;
int angleV[201]={0};
int velocity=0;
int a=1;
int hundCount=0;


void setup() {
  Serial.begin(115200);

  pinMode(RESET_PIN, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(RESET_PIN), reset, FALLING);
  lastSend = millis();
}


void loop() {
  if(millis() >= time_now + period){
      time_now +=period;
      if(a<201){
         a++; //add something here to write time
      }else{
         angleV[0]=angleV[200]; //
        a=1;
      }
      angleV[a]=angle;
      velocity=100*(angleV[a]-angleV[a-1])/period;
      hundCount= millis();
   
      if(time_now>=999 & hundCount%1000==0){
     //Serial.println(millis()); //this works fine
      //Serial.println(velocity); //sort of working kinda slow ask TA if big deal
      //Serial.println(a);
      }
    }
    
  // Read in the current possition
  int newCount = encoder.read();

  // Send the current possition to the pi
  // The raw count data is transmitted to the PI
  // 80 counts per rotation => 40 counts = 3.14
  // to get from counts to radians divide counts by 40 then multiply by 3.14
  // angularPosition = (encoderCounts / 40.0) * 3.14;
  if (((millis() - lastSend) > TARGET_DELAY)) {
    Serial.println(newCount);
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
    String targetPositionStr = Serial.readStringUntil('\n');
    targetPosition = targetPositionStr.toInt();
  }
  Serial.flush();
}

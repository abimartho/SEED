#include <Wire.h>

#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;
int data[8];
int offOne = 0;
int offZero = 0;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(115200); // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Ready!");
}

void loop() {
  delay(100);
}

// callback for received data
void receiveData(int byteCount){
  int i=0;
  while( Wire.available()) {
    data[i] = Wire.read();
    Serial.print(data[i]);
    Serial.print(' ');
    i++;
  }
  i--;
  //Arduino recieved angle stop motors
  if (data[0] == 0) {
    state = 0;
  }
  //Arduino has recieved Angle
  else if (data[0] == 1) {
    state = 1;
    angle = data[1];
  }
  //Arduino has recieved Distance
  else if (data[0] == 2){
    state = 2;
    distance = data[1];
  }
}

// callback for sending data
void sendData(){
  if (state == 0) {
    //Use to indicate that Arduino has stopped and wants Angle
    Wire.write(0);
  }
  else if(state == 1){
    Wire.write(1); //Indicate Arduino is Lined up With Angle
  }
  else if(state == 2){
    Wire.write(2);  //Indiacte Arduino is at Marker
  }
}
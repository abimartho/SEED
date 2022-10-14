#include "Encoder.h"
#include "test2.h"
#include "test3.h"

#define LEFT_PIN_A  2
#define RIGHT_PIN_A 3
#define LEFT_PIN_B  4
#define RIGHT_PIN_B 5

Encoder leftWheel(LEFT_PIN_A, LEFT_PIN_B);
Encoder rightWheel(RIGHT_PIN_A, RIGHT_PIN_B);

typedef enum {TEST2, TEST3} TEST;
TEST currentTest = TEST2;


void setup() {

}

void loop() {
  switch(currentTest){
    case TEST2:
      break;
    case TEST3:
      break;
  }
}

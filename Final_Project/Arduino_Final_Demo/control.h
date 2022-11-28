#include <Encoder.h>
#include <DualMC33926MotorShield.h>

const double kp = 8;
const double kp2 = 32;
const double ki = 0;
const double delta = 150.0;

// Inputs: error between the master wheel and the target possition, error between the maser and slave wheel, pointer to mtrVal array
// Output: updates value in mtrVal array
void drive(double targetError, double wheelError, int mtrVal[2]);

// Inputs: error between the master wheel and the target possition, error between the maser and slave wheel, pointer to mtrVal array
// Output: Updates values in mtrVal array
void turn (int dir, double targetError, double wheelError, int mtrVal[2]);

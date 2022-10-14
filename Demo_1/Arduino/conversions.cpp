// Test 2: Move forward in a straight line and stop after a specified distance in feet (between 1 and 10 feet).

#include "Encoder.h"
#include "test2.h"

// Define the distance from the center to a wheel
#define R 8

int inchesToCounts(int inches) {
  return inches * 173.33;
}

int feetToCounts(double feet) {
  return 2080 * feet
}

int radsToCounts(double rads) {
  return rads * R;
}

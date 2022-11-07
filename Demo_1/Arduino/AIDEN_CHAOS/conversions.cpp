// Test 2: Move forward in a straight line and stop after a specified distance in feet (between 1 and 10 feet).
// Test3: Rotate a specified number of radians then move forward a certain distance
#include "conversions.h"

int inchesToCounts(int inches) {
  return inches * 173.33;
}

double feetToCounts(double feet) {
  return 2080 * feet;
}

int radsToCounts(double rads) {
  return rads * R;
}

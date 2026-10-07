// bdc 0x088305fc BtlHudAngleWithin45
#include "bdc.h"

/* Returns 1 when the angle `a` lies strictly between `centre - π/4` and `centre + π/4`, each bound
   wrapped once by 2π into (−π, π]; 0 otherwise. A wrapped window that straddles ±π is not handled
   (the test is a plain `lo < a < hi`). */
s32 BtlHudAngleWithin45(float a, float centre)
{
  float hi = centre + 0.785398185f;
  float lo = centre - 0.785398185f;

  if (!(hi <= 3.14159274f)) {
    hi -= 6.28318548f;
  } else if (hi <= -3.14159274f) {
    hi += 6.28318548f;
  }
  if (!(lo <= 3.14159274f)) {
    lo -= 6.28318548f;
  } else if (lo <= -3.14159274f) {
    lo += 6.28318548f;
  }
  if (a < hi && !(a <= lo)) {
    return 1;
  }
  return 0;
}

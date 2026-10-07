// bdc 0x088dbcec ActorRadiansToPlacementAngles
#include "bdc.h"

/* Inverse of `ActorPlacementAnglesToRadians`: wraps each angle, converts `pi/2 - a` to 1/65536
   turns and stores three s16s. */

/* Wraps into (-pi, pi]; `!(a <= pi)` mirrors the c.le.s/bc1t test (NaN takes the subtract path). */
static inline float WrapPi(float a)
{
  if (!(a <= 3.1415927f)) {
    a = a - 6.2831855f;
  }
  else if (a <= -3.1415927f) {
    a = a + 6.2831855f;
  }
  return a;
}

static inline s16 RadiansToPlacementAngle(float a)
{
  a = WrapPi(a);
  if (a < 0.0f) {
    a = a + 6.2831855f;
  }
  a = WrapPi(-a + 1.5707964f);
  return (s16)(int)(a * 65535.0f * 0.15915494f);
}

void ActorRadiansToPlacementAngles(s16 *out, const float *angles)
{
  s16 x = RadiansToPlacementAngle(angles[0]);
  s16 y = RadiansToPlacementAngle(angles[1]);
  s16 z = RadiansToPlacementAngle(angles[2]);

  out[0] = x;
  out[1] = y;
  out[2] = z;
}

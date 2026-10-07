// bdc 0x088dbbb0 ActorPlacementAnglesToRadians
#include "bdc.h"

/* Converts three s16 angles (1/65536 turns, placement-record convention) to radians as `pi/2 - a`,
   each wrapped to (-pi, pi], into a vec4 with w = 0. */

void ActorPlacementAnglesToRadians(float *out, const s16 *angles)
{
  float x;
  float y;
  float z;

  x = -((float)angles[0] * 6.2831855f * 1.5259022e-05f - 1.5707964f);
  if (!(x <= 3.1415927f)) {
    x = x - 6.2831855f;
  } else if (x <= -3.1415927f) {
    x = x + 6.2831855f;
  }

  y = -((float)angles[1] * 6.2831855f * 1.5259022e-05f - 1.5707964f);
  if (!(y <= 3.1415927f)) {
    y = y - 6.2831855f;
  } else if (y <= -3.1415927f) {
    y = y + 6.2831855f;
  }

  z = -((float)angles[2] * 6.2831855f * 1.5259022e-05f - 1.5707964f);
  if (!(z <= 3.1415927f)) {
    z = z - 6.2831855f;
  } else if (z <= -3.1415927f) {
    z = z + 6.2831855f;
  }

  out[0] = x;
  out[1] = y;
  out[2] = z;
  out[3] = 0.0f;
}

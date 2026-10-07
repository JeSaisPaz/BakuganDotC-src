// bdc 0x088df800 ActorGetFacingSector
#include "bdc.h"

/* Same classification as `GameGetPlayerFacingSector` for the given actor's heading
   (`base.rot[1]`) against `dir = angle + pi`, using `dir - heading` wrapped into (-pi, pi]:
   returns 1 when within 45 degrees, 0 when beyond 135 degrees (or NaN), otherwise 3 when the
   wrapped difference is positive and 2 when it is not. */

s32 ActorGetFacingSector(float angle, void *actor)
{
  Actor *self = (Actor *)actor;
  float dir;
  float diff;

  dir = angle + 3.1415927f;
  if (!(dir <= 3.1415927f)) {
    dir = dir - 6.2831855f;
  }
  else if (dir <= -3.1415927f) {
    dir = dir + 6.2831855f;
  }
  diff = self->base.rot[1] - dir;
  diff = diff - (float)(s32)(diff * 0.31830987f) * 6.2831855f;
  if (diff < 0.0f) {
    diff = diff + 6.2831855f;
  }
  if (diff < 3.1415927f) {
    diff = -diff;
  }
  else {
    diff = 6.2831855f - diff;
  }
  if (fabsf(diff) < 0.7853982f) {
    return 1;
  }
  if (!(fabsf(diff) <= 2.3561945f)) {
    return 0;
  }
  if (!(diff <= 0.0f)) {
    return 3;
  }
  return 2;
}

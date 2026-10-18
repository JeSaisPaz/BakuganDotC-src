// bdc 0x088fc578 GameQuestCamApplyAccel
#include "bdc.h"

/* Euler step of the velocity of a quest camera spring point: `vel += accel * dt` (xyz, `w` kept),
   then clears the accumulated acceleration `accel` to the zero vector of
   `g_gameQuestCamSpringAxisConsts`. `self` is unused. Helper of `GameQuestCamIntegrate`. */

void GameQuestCamApplyAccel(float dt, GameQuestCamSpring *self, float *vel, float *accel)
{
  float sx = VfMul(accel[0], dt);
  float sy = VfMul(accel[1], dt);
  float sz = VfMul(accel[2], dt);

  vel[0] = VfAdd(vel[0], sx);
  vel[1] = VfAdd(vel[1], sy);
  vel[2] = VfAdd(vel[2], sz);
  *(ScePspFVector4 *)accel = g_gameQuestCamSpringAxisConsts.zero;
}

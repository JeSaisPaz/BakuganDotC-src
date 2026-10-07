// bdc 0x088fc578 GameQuestCamApplyAccel
#include "bdc.h"

/* Euler step of the velocity of a quest camera spring point: `vel += accel * dt` (xyz, `w` kept),
   then clears the accumulated acceleration `accel` to the zero vector of
   `g_gameQuestCamSpringAxisConsts`. `self` is unused. Helper of `GameQuestCamIntegrate`. */

void GameQuestCamApplyAccel(float dt, GameQuestCamSpring *self, float *vel, float *accel)
{
  float sx = accel[0] * dt;
  float sy = accel[1] * dt;
  float sz = accel[2] * dt;

  vel[0] = vel[0] + sx;
  vel[1] = vel[1] + sy;
  vel[2] = vel[2] + sz;
  *(ScePspFVector4 *)accel = g_gameQuestCamSpringAxisConsts.zero;
}

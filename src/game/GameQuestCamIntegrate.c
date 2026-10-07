// bdc 0x088fc5c0 GameQuestCamIntegrate
#include "bdc.h"

/* Semi-implicit Euler integration of a quest camera spring point: applies and clears the
   accumulated acceleration (`GameQuestCamApplyAccel`, `vel += accel*dt`), stores the frame
   displacement `vel*dt` (xyz) in `step` and advances `pos` by it (xyz, `w` kept). */

void GameQuestCamIntegrate(float dt, GameQuestCamSpring *self, float *pos, float *vel, float *accel)
{
  float dx;
  float dy;
  float dz;

  GameQuestCamApplyAccel(dt, self, vel, accel);
  dx = vel[0] * dt;
  dy = vel[1] * dt;
  dz = vel[2] * dt;
  /* step.w gets the stale lane 3 of C710 (never set here): left out. */
  self->step.x = dx;
  self->step.y = dy;
  self->step.z = dz;
  pos[0] = pos[0] + dx;
  pos[1] = pos[1] + dy;
  pos[2] = pos[2] + dz;
}

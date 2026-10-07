// bdc 0x088fc3c8 GameQuestCamSpringAccelerate
#include "bdc.h"

/* Spring step of a quest camera spring point (`GameQuestCamSpringCtor`): when the controller's
   snap flag (`ctrl->snap`) is set, sets the velocity to the `zero` vector of
   `g_gameQuestCamSpringAxisConsts` and writes the goal into `out`; otherwise computes the
   damped-spring acceleration `(goal - out) * (k / (2 * smoothTime))^2 - vel * k` (stiffness `k`,
   the squared factor cached in `springFactor`) and adds `dt` times it to the velocity xyz
   (`vel.w` unchanged; `out` is only read). */

void GameQuestCamSpringAccelerate(float dt, float smoothTime, GameQuestCamSpring *self, float *out)

{
  ScePspFVector4 d;
  ScePspFVector4 v;
  float factor;
  float k;

  if (self->ctrl->snap != '\0') {
    self->vel = g_gameQuestCamSpringAxisConsts.zero;
    out[0] = self->goal.x;
    out[1] = self->goal.y;
    out[2] = self->goal.z;
    out[3] = self->goal.w;
    return;
  }
  d.x = self->goal.x - out[0];
  d.y = self->goal.y - out[1];
  d.z = self->goal.z - out[2];
  factor = self->stiffness / (smoothTime * 2.0f);
  factor = factor * factor;
  self->springFactor = factor;
  d.x = d.x * factor;
  d.y = d.y * factor;
  d.z = d.z * factor;
  v = self->vel;
  k = self->stiffness;
  v.x = v.x * k;
  v.y = v.y * k;
  v.z = v.z * k;
  d.x = d.x - v.x;
  d.y = d.y - v.y;
  d.z = d.z - v.z;
  d.x = d.x * dt;
  d.y = d.y * dt;
  d.z = d.z * dt;
  self->vel.x = self->vel.x + d.x;
  self->vel.y = self->vel.y + d.y;
  self->vel.z = self->vel.z + d.z;
}

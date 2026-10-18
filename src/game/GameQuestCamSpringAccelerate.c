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
  d.x = VfSub(self->goal.x, out[0]);
  d.y = VfSub(self->goal.y, out[1]);
  d.z = VfSub(self->goal.z, out[2]);
  factor = self->stiffness / (smoothTime * 2.0f);
  factor = factor * factor;
  self->springFactor = factor;
  d.x = VfMul(d.x, factor);
  d.y = VfMul(d.y, factor);
  d.z = VfMul(d.z, factor);
  v = self->vel;
  k = self->stiffness;
  v.x = VfMul(v.x, k);
  v.y = VfMul(v.y, k);
  v.z = VfMul(v.z, k);
  d.x = VfSub(d.x, v.x);
  d.y = VfSub(d.y, v.y);
  d.z = VfSub(d.z, v.z);
  d.x = VfMul(d.x, dt);
  d.y = VfMul(d.y, dt);
  d.z = VfMul(d.z, dt);
  self->vel.x = VfAdd(self->vel.x, d.x);
  self->vel.y = VfAdd(self->vel.y, d.y);
  self->vel.z = VfAdd(self->vel.z, d.z);
}

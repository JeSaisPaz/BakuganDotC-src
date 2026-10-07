// bdc 0x088defa8 ActorTurnToward
#include "bdc.h"

/* Turns the actor's heading (base.rot[1]) toward `target` by `rate` x the shortest signed angle
   difference, clamped to +-`maxStep` when non-zero, and wraps the result to (-pi, pi]. Returns the
   signed difference before `rate`/clamp. A difference just beyond -3 rad (in (-3.2, -3)) is flipped
   to avoid oscillating around 180 degrees. */

float ActorTurnToward(float target, float rate, float maxStep, void *actor)
{
  Actor *self = (Actor *)actor;
  float d;
  float diff;
  float step;
  float heading;

  d = self->base.rot[1] - target;
  /* note: divides by pi but subtracts whole turns of 2pi, as the original does */
  d = d - (float)(int)(d * 0.31830987f) * 6.2831855f;
  if (d < 0.0f) {
    d = d + 6.2831855f;
  }
  if (d < 3.1415927f) {
    d = -d;
  }
  else {
    d = 6.2831855f - d;
  }
  diff = d;
  if (d < -3.0f && !(d <= -3.2f)) {
    diff = -d;
  }
  step = diff * rate;
  if (maxStep != 0.0f) {
    if (!(step <= maxStep)) {
      step = maxStep;
    }
    else if (step < -maxStep) {
      step = -maxStep;
    }
  }
  self->base.rot[1] = self->base.rot[1] + step;
  heading = self->base.rot[1];
  if (!(heading <= 3.1415927f)) {
    self->base.rot[1] = heading - 6.2831855f;
    return diff;
  }
  if (heading <= -3.1415927f) {
    self->base.rot[1] = heading + 6.2831855f;
  }
  return diff;
}

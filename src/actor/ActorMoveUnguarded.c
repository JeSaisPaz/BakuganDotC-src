// bdc 0x088de1d0 ActorMoveUnguarded
#include "bdc.h"

/* Resets the recursion depth `g_actorMoveDepth` of `ActorMoveWithCollision`, sets bit 1
   ("moving") in the actor flags `+0x144` and moves the actor by `delta` (4 floats, passed by value). */

void ActorMoveUnguarded(Actor *self, const float *delta)
{
  float step[4] __attribute__((aligned(16)));

  g_actorMoveDepth = 0;
  self->flags = self->flags | 2;
  step[0] = delta[0];
  step[1] = delta[1];
  step[2] = delta[2];
  step[3] = delta[3];
  ActorMoveWithCollision(self, step);
}

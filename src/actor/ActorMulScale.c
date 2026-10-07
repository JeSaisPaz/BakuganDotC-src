// bdc 0x088df968 ActorMulScale
#include "bdc.h"

/* Multiplies the x/y/z of the actor's scale and of its shadow's scale (the shadow's second
   16-byte row) by `factor`. The original stores the whole quad, so each w receives lane 3 of
   C710 (S713), which this function never sets: written as 0.0f (see ## Notes). */

void ActorMulScale(float factor, Actor *actor)
{
  Actor *self = actor;
  float *shadowScale;

  self->base.scale[0] = self->base.scale[0] * factor;
  self->base.scale[1] = self->base.scale[1] * factor;
  self->base.scale[2] = self->base.scale[2] * factor;
  /* UB (original binary): S713, never set here; the sv.q stores it as w. */
  self->base.scale[3] = 0.0f;
  if (self->shadow != 0) {
    shadowScale = ((float(*)[4])self->shadow)[1];
    shadowScale[0] = shadowScale[0] * factor;
    shadowScale[1] = shadowScale[1] * factor;
    shadowScale[2] = shadowScale[2] * factor;
    /* UB (original binary): S713 again. */
    shadowScale[3] = 0.0f;
  }
}

// bdc 0x08859834 ActorCrystalSetHitTimer
#include "bdc.h"

/* Crystal vtable slot 21 (base `0x08863118`): stores `value` at `+0x590` and clears the active-hit
   bits 0 and 2 (`+0x130`) and timer `+0x148` of its collider `+0x20c`. */

void ActorCrystalSetHitTimer(ActorCrystal *self, float value)
{
  self->base.hpThreshold = value;
  self->base.collider0->flags &= ~1u;
  self->base.collider0->hitTimer = 0;
  self->base.collider0->flags &= ~4u;
}

// bdc 0x088deebc ActorIsMotionPlaying
#include "bdc.h"

/* Checks whether the motion in slot `slot` of the actor's motion table (`*(actor+0x150)`, u16
   indices) is the one playing (`GfxModelIsMotion`). */

s32 ActorIsMotionPlaying(Actor *self, s32 slot)
{
  return GfxModelIsMotion(&self->base, (u16)self->motionSlots[slot]);
}

// bdc 0x088deee4 ActorPlayMotion
#include "bdc.h"

/* Starts motion slot `slot` on an actor with blend time `blend` (seconds) and the loop flag:
   returns 0 without restarting when it is already playing unless `force`; otherwise records the
   slot in `+0x348` and starts the motion index from the table `+0x150` (`GfxModelPlayMotion`). Used by
   all actor state handlers (28 callers). */

s32 ActorPlayMotion(float blend, void *actor, s32 slot, u8 loop, u8 force)
{
  Actor *self = (Actor *)actor;

  if (force == 0 && ActorIsMotionPlaying(self, slot) != 0) {
    return 0;
  }
  self->motionSlot = slot;
  return GfxModelPlayMotion(blend, &self->base, (u16)self->motionSlots[slot], loop);
}

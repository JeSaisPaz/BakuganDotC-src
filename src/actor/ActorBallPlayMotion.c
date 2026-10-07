// bdc 0x088b7f98 ActorBallPlayMotion
#include "bdc.h"

/* Starts the motion mapped to `slot` on an `ActorBall` (`ActorBallCtor`) at `speed` through
   `GfxModelPlayMotion` (select motion, then set loop flag `loop`); unless `force` is set it
   returns 0 without restarting when that motion already plays (`ActorBallIsMotionPlaying`). */

s32 ActorBallPlayMotion(float speed, CoreObject *ball, s32 slot, u8 loop, bool force)

{
  ActorBall *b = (ActorBall *)ball;

  if (!force && ActorBallIsMotionPlaying(ball, slot)) {
    return 0;
  }
  return GfxModelPlayMotion(speed, &b->base, b->motionMap[slot], loop);
}

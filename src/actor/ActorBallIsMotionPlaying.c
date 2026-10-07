// bdc 0x088b7f70 ActorBallIsMotionPlaying
#include "bdc.h"

/* Returns whether the `ActorBall`'s current motion (model `+0x138`, `GfxModelIsMotion`) is the motion
   mapped to `slot` in its `+0x144` slot map (`ActorBallCtor`). */

bool ActorBallIsMotionPlaying(CoreObject *ball, s32 slot)

{
  ActorBall *b = (ActorBall *)ball;

  return GfxModelIsMotion(&b->base, b->motionMap[slot]);
}

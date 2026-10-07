// bdc 0x088e3928 ActorPlayerStateTurnToTarget
#include "bdc.h"

/* State 12 of the player actor (edit-man, `ActorPlayerCtor`) (table `0x08a98b44`), staged by
   `waitTimer`: stage 0 turns toward the position of `talkTarget` (`ActorTurnToward`, rate 1,
   max step 8 degrees) and, once the remaining step squared is below 0.01, plays motion slot 9 once
   and advances to stage 1; stage 1 switches to state 8 when that motion has ended. Other stages
   do nothing. */

void ActorPlayerStateTurnToTarget(ActorPlayer *self)

{
  const float *target;
  float step;

  if (self->base.waitTimer > 0) {
    if (self->base.waitTimer < 2 && GfxModelGetMotionRemaining((GfxModel *)self) == 0.0f) {
      ActorSetState(&self->base, 8, 0);
    }
  } else if (self->base.waitTimer >= 0) {
    /* The asm quad-copies the target position to the stack; only x and z are read. */
    target = ((GfxModel *)self->talkTarget)->pos;
    step = ActorTurnToward(atan2f(target[2] - self->base.base.pos[2],
                                  target[0] - self->base.base.pos[0]),
                           1.0f, 0.13962634f, self);
    if (step * step < 0.01f) {
      ActorPlayMotion(0.2f, self, 9, 0, 0);
      self->base.waitTimer = self->base.waitTimer + 1;
    }
  }
}

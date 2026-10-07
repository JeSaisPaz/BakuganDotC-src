// bdc 0x088de20c ActorApplyGravity
#include "bdc.h"

/* Vertical movement step of an actor (called by `ActorUpdate`, `ActorPlayerUpdate` and
   `ActorNpcUpdate`): while the jump timer `+0x33c` runs the fall speed `+0x250` is damped by 0.8
   per frame, otherwise it eases toward 2.3; unless the actor already moved this frame (flag bit 1
   of `+0x144`) the speed is subtracted from the velocity y `+0x84`, the velocity `+0x80` is applied
   with `ActorMoveUnguarded`, and the airborne frame counter `+0x160` is advanced (bit 30 set) or
   reset together with the velocity y (landed). */

void ActorApplyGravity(Actor *self)
{
  float delta[4] __attribute__((aligned(16)));

  if (self->jumpTimer < 1) {
    self->fallSpeed = self->fallSpeed + (2.3f - self->fallSpeed) * 0.2f;
  }
  else {
    self->jumpTimer = self->jumpTimer - 1;
    self->fallSpeed = self->fallSpeed * 0.8f;
  }
  if ((self->flags & 2) == 0) {
    self->base.velocity[1] = self->base.velocity[1] - self->fallSpeed;
    delta[0] = self->base.velocity[0];
    delta[1] = self->base.velocity[1];
    delta[2] = self->base.velocity[2];
    delta[3] = self->base.velocity[3];
    ActorMoveUnguarded(self, delta);
    if ((self->flags & 0x40000000) == 0) {
      self->airFrames = 0;
      self->base.velocity[1] = 0.0f;
    }
    else {
      self->airFrames = self->airFrames + 1;
    }
  }
}

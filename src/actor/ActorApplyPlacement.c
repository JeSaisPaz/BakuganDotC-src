// bdc 0x088df598 ActorApplyPlacement
#include "bdc.h"

/* Moves the actor to its placement record `+0x350`: position = record ints x 20/4096, rotation from
   the record's s16 angles (`ActorPlacementAnglesToRadians`) into `+0x30`, then plays the placed
   idle motion (`ActorPlayPlacedMotion`). */

void ActorApplyPlacement(Actor *self)
{
  float v[4] __attribute__((aligned(16)));
  const s32 *rec;

  if (self->placement != (void *)0x0) {
    rec = (const s32 *)self->placement;
    v[0] = (float)rec[0] * 0.00024414062f;
    v[1] = (float)rec[1] * 0.00024414062f;
    v[2] = (float)rec[2] * 0.00024414062f;
    v[3] = 0.0f;
    /* vscl.t: only x/y/z are scaled; pos.w receives a stale VFPU lane (left unchanged here) */
    self->base.pos[0] = v[0] * 20.0f;
    self->base.pos[1] = v[1] * 20.0f;
    self->base.pos[2] = v[2] * 20.0f;
    ActorPlacementAnglesToRadians(v, (const s16 *)(rec + 3));
    self->base.rot[0] = v[0];
    self->base.rot[1] = v[1];
    self->base.rot[2] = v[2];
    self->base.rot[3] = v[3];
    ActorPlayPlacedMotion(self, 0);
  }
}

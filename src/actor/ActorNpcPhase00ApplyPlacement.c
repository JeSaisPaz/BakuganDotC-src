// bdc 0x088e60b4 ActorNpcPhase00ApplyPlacement
#include "bdc.h"

/* Setup phase 0 of the field NPC/guard classes (base `ActorNpcCtor`) (table `0x08a98cdc`):
   applies the placement record (position x 20/4096, w 0; angles via
   `ActorNpcPlacementAnglesToRadians`) and advances the phase; sets the collision step `+0x158` to
   1.4934 in every case. */

void ActorNpcPhase00ApplyPlacement(ActorNpc *self)
{
  float v[4];
  const ActorNpcPlacement *rec;

  if (self->base.placement != (void *)0x0) {
    rec = (const ActorNpcPlacement *)self->base.placement;
    v[0] = (float)rec->pos[0] * 0.00024414062f;
    v[1] = (float)rec->pos[1] * 0.00024414062f;
    v[2] = (float)rec->pos[2] * 0.00024414062f;
    v[3] = 0.0f;
    /* vscl.t into C710; lane 3 stored by sv.q is the bank zero S713 */
    self->base.base.pos[0] = v[0] * 20.0f;
    self->base.base.pos[1] = v[1] * 20.0f;
    self->base.base.pos[2] = v[2] * 20.0f;
    self->base.base.pos[3] = 0.0f;
    ActorNpcPlacementAnglesToRadians(v, rec->angles);
    self->base.base.rot[0] = v[0];
    self->base.base.rot[1] = v[1];
    self->base.base.rot[2] = v[2];
    self->base.base.rot[3] = v[3];
    self->phase = self->phase + 1;
  }
  self->base.collisionMask = 0x3fbf2700;
}

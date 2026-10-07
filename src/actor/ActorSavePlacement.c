// bdc 0x088df44c ActorSavePlacement
#include "bdc.h"

/* Writes the actor's position (`+0x20` / 20, in 1/4096 units, rounded half away from zero) and
   rotation (`+0x30`, converted with `ActorRadiansToPlacementAngles`) back into its placement
   record `+0x350`. Does nothing without a record. */

static s32 ActorSavePlacementRound(float x)
{
  if (x <= 0.0f) {
    return (s32)(x * 4096.0f - 0.5f);
  }
  return (s32)(x * 4096.0f + 0.5f);
}

void ActorSavePlacement(Actor *self)
{
  float v[3];
  float rot[4];
  s16 angles[3];
  s32 *rec;
  s16 *recAngles;
  s32 x;
  s32 y;
  s32 z;

  if (self->placement != (void *)0x0) {
    rec = (s32 *)self->placement;
    /* vscl.t: pos * 0.05f (lane 3 of the stored quad is stale and never read) */
    v[0] = self->base.pos[0] * 0.05f;
    v[1] = self->base.pos[1] * 0.05f;
    v[2] = self->base.pos[2] * 0.05f;
    x = ActorSavePlacementRound(v[0]);
    y = ActorSavePlacementRound(v[1]);
    z = ActorSavePlacementRound(v[2]);
    rec[0] = x;
    rec[1] = y;
    rec[2] = z;
    rot[0] = self->base.rot[0];
    rot[1] = self->base.rot[1];
    rot[2] = self->base.rot[2];
    rot[3] = self->base.rot[3];
    recAngles = (s16 *)((s32 *)self->placement + 3);
    ActorRadiansToPlacementAngles(angles, rot);
    recAngles[0] = angles[0];
    recAngles[1] = angles[1];
    recAngles[2] = angles[2];
  }
}

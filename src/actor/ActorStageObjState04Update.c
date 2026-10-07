// bdc 0x088ae75c ActorStageObjState04Update
#include "bdc.h"

/* State 4 handler of the shared stage-object state machine (state `+0x304`, `MemberFnPtr` table
   `0x08a842f8`, run by `ActorStageObjUpdate`): break state. On the first frame (`step` 0) spawns
   the break effects attached to the object (`GfxEffectSpawnWithOwner` on `g_btlUnitEffectMgr`): when
   virtual `+0x68` returns non-zero, one 0x160 at the bounds' min-Y and two 0x160 at random points in
   the bounds box, otherwise six 0x8f at random points. Every frame it then sinks the model
   (`restHeight` and `pos.y` drop by `height * 0.0444` while `restHeight` is above two thirds of the
   height, then by `height * 0.01111` while shaking X/Z around `shakeX`/`shakeZ` with the shake
   table); once `pos.y` is below `baseY - 0.3 * height` it builds the collapse model and fades out by
   0.02 per frame; at `fade <= 0.2` the collider turns non-blocking (flag 2) and remains are left, at
   `fade <= 0` the object is marked for removal. Finally flags the collider's attach matrix dirty. */

/* Random float in [0, 1): vrndf1 gives [1, 2), minus the bank constant S733 = 1.0f. */
static inline float State04Random(void)
{
  return PlatformRandFloat12() - 1.0f;
}

/* lv.q/sv.q copy of the model position (all four lanes). */
static inline void State04CopyPos(float *dst, const float *src)
{
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

/* pos[i] += max[i] * 2 * rand - max[i] for X, Y, Z, re-reading the bounds each time. */
static void State04Scatter(ActorStageObjBase *self, float *pos)
{
  int i;

  for (i = 0; i < 3; i++) {
    float p = pos[i];
    float m = ActorStageObjGetBounds(self)[4 + i] * 2.0f;
    float r = State04Random();
    pos[i] = p + (m * r - ActorStageObjGetBounds(self)[4 + i]);
  }
}

void ActorStageObjState04Update(ActorStageObjBase *self)
{
  float shakeX;
  float shakeZ;
  float drop;
  int off;
  int i;

  if (self->step == 0) {
    const MemberFnPtr *fn = (const MemberFnPtr *)self->base.base.vtable + 13; /* vtable +0x68 */

    if (((int (*)(void *))fn->pfn)((char *)self + fn->delta) != 0) {
      float pos[4] __attribute__((aligned(16)));

      State04CopyPos(pos, self->base.pos);
      pos[1] = pos[1] + ActorStageObjGetBounds(self)[1];
      GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 0x160, pos, self);
      for (i = 0; i < 2; i++) {
        float p[4] __attribute__((aligned(16)));

        State04CopyPos(p, self->base.pos);
        State04Scatter(self, p);
        GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 0x160, p, self);
      }
    } else {
      for (i = 0; i < 6; i++) {
        float p[4] __attribute__((aligned(16)));

        State04CopyPos(p, self->base.pos);
        State04Scatter(self, p);
        GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 0x8f, p, self);
      }
    }
    self->step = self->step + 1;
  }

  if (!(self->restHeight <= self->height * 2.0f * 0.33333334f)) {
    drop = self->height * 0.0444f;
    self->restHeight = self->restHeight - drop;
    self->base.pos[1] = self->base.pos[1] - drop;
  } else {
    shakeX = self->shakeX;
    off = ActorStageObjGetShakeOffset(self, self->shakePhase & 0x1f);
    shakeZ = self->shakeZ;
    self->base.pos[0] = shakeX + (float)(off / 2);
    off = ActorStageObjGetShakeOffset(self, (self->shakePhase + 8) & 0x1f);
    drop = self->height * 0.01111f;
    self->shakePhase = self->shakePhase + 1;
    self->base.pos[2] = shakeZ + (float)(off / 2);
    self->restHeight = self->restHeight - drop;
    self->base.pos[1] = self->base.pos[1] - drop;
  }

  if (self->base.pos[1] < self->baseY - self->height * 0.3f) {
    ActorStageObjSpawnBreakModel(self);
    self->fade = self->fade - 0.02f;
  }
  if (self->fade <= 0.0f) {
    self->fade = 0.0f;
    self->removeRequest = 1;
  } else if (self->fade <= 0.2f) {
    ((CollisionCollider *)self->collider)->flags |= 2;
    ActorStageObjSpawnRemains(self);
  }
  if (self->collider != NULL) {
    ((CollisionCollider *)self->collider)->attachDirty = 1;
  }
}

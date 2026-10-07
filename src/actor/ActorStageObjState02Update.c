// bdc 0x088adf54 ActorStageObjState02Update
#include "bdc.h"

/* State 2 handler of the shared stage-object state machine (state `+0x304`, `MemberFnPtr` table
   `0x08a842f8`, run by `ActorStageObjUpdate`): hit-and-collapse state. First, while the object is
   visible with ambient alpha <= 0, every effect of `g_btlUnitEffectMgr` owned by it is released
   (`UiSpriteLayerRelease` on the effect's manager). Then by `step`:
   - 0: spawns `clamp((int)(diagonal2 * 0.01), 1, 8)` pairs of effects 0x8e / 0x8d at random X/Z
     points of the bounds box around the model centre (`GfxEffectSpawnWithOwner`), sets `timer` 10
     and advances, then shakes like step 1.
   - 1: while `timer` is non-zero shakes X/Z around `shakeX`/`shakeZ` with the shake table and counts
     down; at 0 spawns `clamp((int)(diagonal2 * 0.005), 1, 8)` effects 0x8f, turns lighting on, makes
     the materials translucent, sets `timer` 5, advances and runs step 2 in the same frame.
   - 2: sinks the model (`restHeight` and `pos.y` drop by `height * 0.0444` while `restHeight` is above
     two thirds of the height, then by `height * 0.01111` while shaking); once `pos.y` is below
     `baseY - 0.3 * height` builds the collapse model and fades by 0.02 per frame; flags the
     collider's attach matrix dirty. At `fade <= 0` it clamps fade to 0, spawns effect 8 at the
     centre, sets `timer` 250 and advances; at `fade <= 0.2` the collider turns non-blocking
     (flag 2) and remains are left; otherwise every 6th frame (timer 5) spawns effect 0x91 at a
     random X/Z point.
   - 3: counts `timer` down, then sets `removeRequest`.
   Other steps do nothing. */

/* Random float in [0, 1): vrndf1 gives [1, 2), minus the bank constant S733 = 1.0f. */
static inline float State02Random(void)
{
  return PlatformRandFloat12() - 1.0f;
}

/* lv.q/sv.q copy of a 16-byte position (all four lanes). */
static inline void State02CopyPos(float *dst, const float *src)
{
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

/* pos.x/pos.z += max * 2 * rand - max, re-reading the bounds each time. */
static void State02ScatterXZ(ActorStageObjBase *self, float *pos)
{
  int i;

  for (i = 0; i < 3; i += 2) {
    float p = pos[i];
    float m = ActorStageObjGetBounds(self)[4 + i] * 2.0f;
    float r = State02Random();
    pos[i] = p + (m * r - ActorStageObjGetBounds(self)[4 + i]);
  }
}

/* Model position raised by the bounds' min Y. */
static void State02Centre(ActorStageObjBase *self, float *pos)
{
  float y;

  State02CopyPos(pos, self->base.pos);
  y = pos[1];
  pos[1] = y + ActorStageObjGetBounds(self)[1];
}

/* X/Z jitter from the shake table around shakeX/shakeZ; advances shakePhase. */
static void State02Shake(ActorStageObjBase *self)
{
  float shakeX;
  float shakeZ;
  int off;

  shakeX = self->shakeX;
  off = ActorStageObjGetShakeOffset(self, self->shakePhase & 0x1f);
  shakeZ = self->shakeZ;
  self->base.pos[0] = shakeX + (float)(off / 2);
  off = ActorStageObjGetShakeOffset(self, (self->shakePhase + 8) & 0x1f);
  self->shakePhase = self->shakePhase + 1;
  self->timer = self->timer - 1;
  self->base.pos[2] = shakeZ + (float)(off / 2);
}

void ActorStageObjState02Update(ActorStageObjBase *self)
{
  float pos[4] __attribute__((aligned(16)));
  float shakeX;
  float shakeZ;
  float drop;
  int off;
  int count;
  int i;

  if (self->visible != 0 && self->base.ambient[3] <= 0.0f) {
    GfxEffect *fx = (GfxEffect *)g_btlUnitEffectMgr->base.head;

    while (fx != NULL) {
      GfxEffect *next = (GfxEffect *)fx->base.next;

      if (fx->ownerBakugan == self) {
        UiSpriteLayerRelease(fx->mgr, fx);
      }
      fx = next;
    }
  }

  switch (self->step) {
  case 0:
    State02Centre(self, pos);
    count = (int)(self->diagonal2 * 0.01f);
    if (count < 1) {
      count = 1;
    } else if (8 < count) {
      count = 8;
    }
    for (i = 0; i < count; i++) {
      float p[4] __attribute__((aligned(16)));

      State02CopyPos(p, pos);
      State02ScatterXZ(self, p);
      GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 0x8e, p, self);
      State02CopyPos(p, pos);
      State02ScatterXZ(self, p);
      GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 0x8d, p, self);
    }
    self->timer = 10;
    self->step = self->step + 1;
    /* fall through */
  case 1:
    if (self->timer != 0) {
      State02Shake(self);
      return;
    }
    State02Centre(self, pos);
    count = (int)(self->diagonal2 * 0.005f);
    if (count < 1) {
      count = 1;
    } else if (8 < count) {
      count = 8;
    }
    for (i = 0; i < count; i++) {
      float p[4] __attribute__((aligned(16)));

      State02CopyPos(p, pos);
      State02ScatterXZ(self, p);
      GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 0x8f, p, self);
    }
    self->base.lighting = 1;
    GfxModelForEachMaterial(&self->base, ActorStageObjMaterialSetTranslucent, NULL);
    self->timer = 5;
    self->step = self->step + 1;
    /* fall through */
  case 2:
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
    if (self->collider != NULL) {
      ((CollisionCollider *)self->collider)->attachDirty = 1;
    }

    if (self->fade <= 0.0f) {
      self->fade = 0.0f;
      State02Centre(self, pos);
      self->timer = 250;
      GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 8, pos, self);
      self->step = self->step + 1;
    } else if (self->fade <= 0.2f) {
      ((CollisionCollider *)self->collider)->flags |= 2;
      ActorStageObjSpawnRemains(self);
    } else if (self->timer != 0) {
      self->timer = self->timer - 1;
    } else {
      State02Centre(self, pos);
      State02ScatterXZ(self, pos);
      GfxEffectSpawnWithOwner(g_btlUnitEffectMgr, 0x91, pos, self);
      self->timer = 5;
    }
    break;
  case 3:
    if (self->timer != 0) {
      self->timer = self->timer - 1;
    } else {
      self->removeRequest = 1;
    }
    break;
  default:
    break;
  }
}

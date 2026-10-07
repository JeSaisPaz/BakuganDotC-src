// bdc 0x08858054 ActorCrystalUpdateFade
#include "bdc.h"

/* Crystal vanish/reappear sequence, one step per call while `fadeEnabled` is set (step in
   `fadeStep`, frame counter in `fadeTimer`):
   - 1..3: after a short wait, spawns effect 0x5c at the crystal (`effectPos`), plays sound
     `0x20025d` and turns lighting on;
   - 4/10: fades the model out along sin(pi/2 - t*5deg) (`GfxModelScaleAmbientColor`, the
     `"mat_spel"` materials at half the factor via `GfxModelScaleAmbientColorByName`, the stand
     model alike);
   - 11: once invisible, makes it untargetable and releases links to it when script bit 0x23 is
     set, drops the player's link to it, moves it 5000 units down and disables both colliders;
   - 12: waits while script bit 0x23 is set, then 10 frames;
   - 20/21: snaps `warpPos` to the ground (`CollisionRaycastPoint`), moves the crystal there
     (also `homePos`/`basePos`) and spawns the attached effect 0x5d;
   - 30: fades back in along sin(t*5deg), targetable again, colliders re-enabled;
   - 31 (and 999): full opacity, lighting off; ends the sequence (`fadeDone = 1`) when
     `fadeOneShot` is set, otherwise restarts at step 1.
   The sine is vsin of angle * S703 (2/pi), i.e. sinf(angle) in radians. */

#define CRYSTAL_FADE_STEP_ANGLE 0.08726646f /* 5 degrees */
#define CRYSTAL_HALF_PI 1.5707964f

/* 16-byte vec4 copy (the listing's lv.q/sv.q pair). */
static void ActorCrystalFadeCopyVec4(float *dst, const float *src)
{
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

void ActorCrystalUpdateFade(ActorCrystal *self)
{
  GfxEffect *effect;
  void *player;
  CollisionCollider *collider;
  float angle;
  float alpha;
  s32 t;

  if (self->fadeEnabled == 0) {
    return;
  }
  switch (self->fadeStep) {
  case 1:
    self->fadeTimer = 0;
    self->fadeStep = self->fadeStep + 1;
    /* fall through */
  case 2:
    self->fadeTimer = self->fadeTimer - 1;
    if (self->fadeTimer > 0) {
      return;
    }
    self->fadeTimer = 0;
    self->fadeStep = self->fadeStep + 1;
    /* fall through */
  case 3:
    ActorCrystalFadeCopyVec4(self->effectPos, self->base.base.pos);
    effect = (GfxEffect *)GfxEffectSpawn(g_btlUnitEffectMgr, 0x5c, self->effectPos);
    effect->vec1e0[0] = self->base.base.scale[0] * 0.4f;
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x20025d, 0, 0);
    }
    self->base.base.lighting = 1;
    self->fadeTimer = 1;
    self->fadeStep = self->fadeStep + 1;
    /* fall through */
  case 4:
    self->fadeTimer = self->fadeTimer - 1;
    if (self->fadeTimer > 0) {
      return;
    }
    self->fadeTimer = 0;
    self->fadeStep = 10;
    /* fall through */
  case 10:
    t = self->fadeTimer;
    angle = CRYSTAL_HALF_PI - (float)t * CRYSTAL_FADE_STEP_ANGLE;
    self->fadeTimer = t + 1;
    if (angle < 0.0f) {
      angle = 0.0f;
    } else if (!(angle <= CRYSTAL_HALF_PI)) {
      angle = CRYSTAL_HALF_PI;
    }
    alpha = __builtin_sinf(angle);
    self->base.base.ambient[3] = alpha;
    GfxModelScaleAmbientColor(alpha, &self->base.base, NULL);
    GfxModelScaleAmbientColorByName(alpha * 0.5f, &self->base.base, "mat_spel");
    self->fade = alpha;
    if (self->stand != NULL) {
      ((ActorCrystalStand *)self->stand)->base.ambient[3] = alpha;
    }
    if (!(alpha <= 0.0f)) {
      return;
    }
    self->fadeStep = self->fadeStep + 1;
    /* fall through */
  case 11:
    if (CoreBitsetTest(0x23, g_scriptGlobalBits)) {
      self->base.untargetable = 1;
      BtlBakuganReleaseLinksTo(self);
    }
    player = BtlGetPlayerBakugan();
    if (BtlBakuganGetTarget(player) == self) {
      BtlBakuganClearLink(player);
    }
    self->base.base.ambient[3] = 0.0f;
    self->fadeTimer = 10;
    self->base.base.pos[1] = self->base.base.pos[1] - 5000.0f;
    ActorCrystalFadeCopyVec4(&self->base.base.data->rootMatrix[12], self->base.base.pos);
    collider = self->base.collider0;
    collider->flags = collider->flags | 1;
    collider->hitTimer = 0;
    self->base.collider0->flags = self->base.collider0->flags | 0x40;
    self->base.collider0->flags = self->base.collider0->flags | 4;
    collider = self->base.collider1;
    collider->flags = collider->flags | 1;
    collider->hitTimer = 0;
    self->base.collider1->flags = self->base.collider1->flags | 0x40;
    self->base.collider1->flags = self->base.collider1->flags | 4;
    self->fadeStep = self->fadeStep + 1;
    /* fall through */
  case 12:
    if (CoreBitsetTest(0x23, g_scriptGlobalBits)) {
      return;
    }
    self->fadeTimer = self->fadeTimer - 1;
    if (self->fadeTimer > 0) {
      return;
    }
    self->fadeStep = 20;
    /* fall through */
  case 20:
    CollisionRaycastPoint(self->warpPos, self->warpPos);
    ActorCrystalFadeCopyVec4(self->base.base.pos, self->warpPos);
    ActorCrystalFadeCopyVec4(self->homePos, self->base.base.pos);
    ActorCrystalFadeCopyVec4(self->basePos, self->homePos);
    ActorCrystalFadeCopyVec4(self->attachPos, self->base.base.pos);
    effect = (GfxEffect *)GfxEffectSpawnAttached(g_btlUnitEffectMgr, 0x5d, self->attachPos);
    effect->vec1e0[0] = self->base.base.scale[0] * 0.4f;
    self->fadeTimer = 1;
    self->fadeStep = self->fadeStep + 1;
    /* fall through */
  case 21:
    self->fadeTimer = self->fadeTimer - 1;
    if (self->fadeTimer > 0) {
      return;
    }
    self->fadeTimer = 0;
    self->fadeStep = 30;
    /* fall through */
  case 30:
    t = self->fadeTimer;
    angle = (float)t * CRYSTAL_FADE_STEP_ANGLE;
    self->fadeTimer = t + 1;
    if (angle < 0.0f) {
      angle = 0.0f;
    } else if (!(angle <= CRYSTAL_HALF_PI)) {
      angle = CRYSTAL_HALF_PI;
    }
    alpha = __builtin_sinf(angle);
    self->base.base.ambient[3] = alpha;
    GfxModelScaleAmbientColor(alpha, &self->base.base, NULL);
    GfxModelScaleAmbientColorByName(alpha * 0.5f, &self->base.base, "mat_spel");
    self->fade = alpha;
    self->base.untargetable = 0;
    collider = self->base.collider0;
    collider->flags = collider->flags & ~1u;
    collider->hitTimer = 0;
    self->base.collider0->flags = self->base.collider0->flags & ~0x40u;
    self->base.collider0->flags = self->base.collider0->flags & ~4u;
    self->base.collider1->flags = self->base.collider1->flags & ~0x40u;
    self->base.collider1->flags = self->base.collider1->flags & ~4u;
    if (self->stand != NULL) {
      ((ActorCrystalStand *)self->stand)->base.ambient[3] = alpha;
      ActorCrystalStandSyncCollider((ActorCrystalStand *)self->stand);
    }
    if (alpha < 1.0f) {
      return;
    }
    self->fadeStep = self->fadeStep + 1;
    /* fall through */
  case 31:
    self->base.base.ambient[3] = 1.0f;
    GfxModelScaleAmbientColor(1.0f, &self->base.base, NULL);
    GfxModelScaleAmbientColorByName(0.5f, &self->base.base, "mat_spel");
    self->fade = 1.0f;
    if (self->stand != NULL) {
      ((ActorCrystalStand *)self->stand)->base.ambient[3] = 1.0f;
    }
    self->base.base.lighting = 0;
    self->fadeStep = self->fadeStep + 1;
    break;
  case 999:
    break;
  default:
    return;
  }

  if (self->fadeOneShot != 0) {
    self->fadeStep = 0;
    self->fadeDone = 1;
    self->fadeOneShot = 0;
    self->fadeEnabled = 0;
  } else {
    self->fadeStep = 1;
  }
}

// bdc 0x088a7820 ActorStageObjAttrLandmarkUpdateFade
#include "bdc.h"

/* Updates the attribute landmark's see-through alpha `baseAlpha` (max 0.95): does nothing once
   `dead` is set; forced to 1 (and applied) while the battle main task (task 100, `BtlMain`) is
   in phase 6 or 9; otherwise runs `ActorStageObjAttrLandmarkTestOcclusion` against the player
   Bakugan (`BtlGetPlayerBakugan`): occlusion state 1 fades in by 0.1 (max 0.95), 2 sets 0,
   3 fades out by 0.1 (min 0), any other state sets 0.95 unless the test returned non-zero.
   Applies the result with `GfxModelScaleAmbientColor` when it changed. */

void ActorStageObjAttrLandmarkUpdateFade(ActorStageObjAttrLandmark *self)
{
  BtlMain *main;
  void *unit;
  int hit;
  float prev;
  float alpha;
  u8 occl;

  if (self->base.dead != 0) {
    return;
  }
  main = (BtlMain *)CoreTaskFind(100);
  if (main != NULL && (main->phase == 9 || main->phase == 6)) {
    self->base.baseAlpha = 1.0f;
    GfxModelScaleAmbientColor(1.0f, &self->base.base, NULL);
    return;
  }
  prev = self->base.baseAlpha;
  unit = BtlGetPlayerBakugan();
  occl = 0;
  hit = ActorStageObjAttrLandmarkTestOcclusion(self, unit, &self->base.baseAlpha, (char *)&occl);
  switch (occl) {
  case 1:
    alpha = self->base.baseAlpha + 0.1f;
    self->base.baseAlpha = alpha;
    if (!(alpha <= 0.95f)) {
      self->base.baseAlpha = 0.95f;
    }
    break;
  case 2:
    self->base.baseAlpha = 0.0f;
    break;
  case 3:
    alpha = self->base.baseAlpha - 0.1f;
    self->base.baseAlpha = alpha;
    if (alpha < 0.0f) {
      self->base.baseAlpha = 0.0f;
    }
    break;
  default:
    if (hit == 0) {
      self->base.baseAlpha = 0.95f;
    }
    break;
  }
  if (self->base.baseAlpha != prev) {
    GfxModelScaleAmbientColor(self->base.baseAlpha, &self->base.base, NULL);
  }
}

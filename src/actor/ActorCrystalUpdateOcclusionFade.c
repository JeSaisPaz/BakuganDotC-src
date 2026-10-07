// bdc 0x08857434 ActorCrystalUpdateOcclusionFade
#include "bdc.h"

/* Makes the crystal see-through while it hides the player's Bakugan (`ActorCrystalTestOccludes`):
   eases the alpha `+0xa80` back to 1 when clear and, when it changed, applies it to the model's
   vertex colours and half of it to the `"mat_spel"` material (`GfxModelScaleAmbientColor`,
   `GfxModelScaleAmbientColorByName`). Skipped while `+0x940` or `+0xa3a` is set. */

void ActorCrystalUpdateOcclusionFade(ActorCrystal *self)
{
  void *unit;
  int occluded;
  float alpha;
  float prevAlpha;
  u8 fadeMode[4];

  if (self->fadeSkip != 0) {
    return;
  }
  if (self->onstageFlag != 0) {
    return;
  }
  prevAlpha = self->alpha;
  unit = BtlGetPlayerBakugan();
  fadeMode[0] = 0;
  occluded = ActorCrystalTestOccludes(self, unit, &self->alpha, fadeMode);
  switch (fadeMode[0]) {
  case 1: /* fade back in */
    alpha = self->alpha + 0.1f;
    self->alpha = alpha;
    if (!(alpha <= 1.0f)) {
      self->alpha = 1.0f;
    }
    break;
  case 2:
    self->alpha = 0.0f;
    break;
  case 3: /* fade out */
    alpha = self->alpha - 0.1f;
    self->alpha = alpha;
    if (alpha < 0.0f) {
      self->alpha = 0.0f;
    }
    break;
  default:
    if (occluded == 0) {
      self->alpha = 1.0f;
    }
    break;
  }
  if (self->alpha != prevAlpha) {
    GfxModelScaleAmbientColor(self->alpha, (GfxModel *)self, NULL);
    GfxModelScaleAmbientColorByName(self->alpha * 0.5f, (GfxModel *)self, "mat_spel");
  }
}

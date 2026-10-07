// bdc 0x08859e60 ActorCrystalResetTintAndStopFade
#include "bdc.h"

/* Same colour reset as `ActorCrystalResetTintAndStartFade` (`ambient[3] = 1.0`, ambient ×1.0,
   `"mat_spel"` ×0.5, `fade = 1.0`, mirrored into the stand `+0x6bc`), then stops the fade
   sequence of `ActorCrystalUpdateFade`: clears `lighting`, the step `fadeStep` and the enable
   byte `fadeEnabled`. */

void ActorCrystalResetTintAndStopFade(ActorCrystal *self)

{
  (self->base).base.ambient[3] = 1.0f;
  GfxModelScaleAmbientColor(1.0f, (GfxModel *)self, NULL);
  GfxModelScaleAmbientColorByName(0.5f, (GfxModel *)self, "mat_spel");
  self->fade = 1.0f;
  if (self->stand != NULL) {
    ((GfxModel *)self->stand)->ambient[3] = 1.0f;
  }
  (self->base).base.lighting = 0;
  self->fadeStep = 0;
  self->fadeEnabled = 0;
}

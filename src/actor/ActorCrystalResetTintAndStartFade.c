// bdc 0x08859ddc ActorCrystalResetTintAndStartFade
#include "bdc.h"

/* Resets the crystal's model colour and starts a one-shot fade: ambient alpha `+0x6c = 1.0`, all
   ambient colours ×1.0 (`GfxModelScaleAmbientColor`), the `"mat_spel"` material ×0.5
   (`GfxModelScaleAmbientColorByName`), the same alpha mirrored into the stand at `+0x6bc`, fade
   value `+0xa84 = 1.0`; then `ActorCrystalStartFade`, `fadeOneShot = 1` and `fadeDone = 0`. */

void ActorCrystalResetTintAndStartFade(ActorCrystal *self)
{
    self->base.base.ambient[3] = 1.0f;
    GfxModelScaleAmbientColor(1.0f, (GfxModel *)self, NULL);
    GfxModelScaleAmbientColorByName(0.5f, (GfxModel *)self, "mat_spel");
    if (self->stand != NULL) {
        ((GfxModel *)self->stand)->ambient[3] = 1.0f;
    }
    self->fade = 1.0f;
    ActorCrystalStartFade(self);
    self->fadeOneShot = 1;
    self->fadeDone = 0;
}

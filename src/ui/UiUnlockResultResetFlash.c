// bdc 0x0893b930 UiUnlockResultResetFlash
#include "bdc.h"

/* When a reveal flash is requested (`+0x7f8`) on `UiUnlockResult`, resets its
   counters (`+0x7f0`, `+0x7f4`) and the burst sprite 0x24 (scale 0, alpha 0.3, visible). */

void UiUnlockResultResetFlash(UiUnlockResult *self)
{
    GfxSprite **sprites;

    if (self->flashRequested != 0) {
        self->flashTimer = 0;
        sprites = (GfxSprite **)self->base.data;
        self->flashScale = 0;
        GfxSpriteSetScaleRotation(sprites[0x24], 0.0f, 0.0f, 0.0f, false);
        ((GfxSprite **)self->base.data)[0x24]->alpha = 0.3f;
        ((GfxSprite **)self->base.data)[0x24]->flags |= 1;
    }
}

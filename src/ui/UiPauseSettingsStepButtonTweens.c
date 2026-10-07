// bdc 0x089ac50c UiPauseSettingsStepButtonTweens
#include "bdc.h"

/* Steps the six button-row tweens (`buttonTweens`, +0x848) of sprites 0x32..0x37 by one frame
   with `UiTweenUpdate` (flags 3, 16 frames): opening scales 1.5 -> 1, closing 1 -> 1.5 and fades
   out. Returns 1 when at least one tween reported finished (the u8 count of finished tweens is
   non-zero), else 0. */

int UiPauseSettingsStepButtonTweens(UiPauseSettings *self, u8 closing)
{
    GfxSprite **sprites;
    u8 finished = 0;
    int i;

    if (closing == 0) {
        for (i = 0x32; i < 0x38; i++) {
            sprites = (GfxSprite **)self->base.data;
            finished += UiTweenUpdate(1.5f, 1.0f, 16.0f, closing, sprites[i],
                                      &self->buttonTweens[i - 0x32], 3);
        }
    } else {
        for (i = 0x32; i < 0x38; i++) {
            sprites = (GfxSprite **)self->base.data;
            finished += UiTweenUpdate(1.0f, 1.5f, 16.0f, closing, sprites[i],
                                      &self->buttonTweens[i - 0x32], 3);
        }
    }
    return finished != 0;
}

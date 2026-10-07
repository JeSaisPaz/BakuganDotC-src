// bdc 0x089ac138 UiPauseSettingsStartBgFade
#include "bdc.h"

/* Prepares the fade of the background panel (sprite `data+0x64`) and the alpha `+0xb88`: shown from
   0 when opening, from 1 when `closing`. Stepped by `UiPauseSettingsStepBgFade`. */

void UiPauseSettingsStartBgFade(UiPauseSettings *self, u8 closing)
{
    if (closing == 0) {
        GfxSprite *bg;

        self->bgAlpha = 0.0f;
        bg = ((GfxSprite **)self->base.data)[25];
        bg->flags |= 1;
        self->bgFadeT = 0.0f;
        self->bgFadeFrom = 0.0f;
        return;
    }
    self->bgFadeT = 0.0f;
    self->bgAlpha = 1.0f;
    self->bgFadeFrom = 1.0f;
}

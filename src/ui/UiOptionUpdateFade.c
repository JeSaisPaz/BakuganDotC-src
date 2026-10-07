// bdc 0x08970478 UiOptionUpdateFade
#include "bdc.h"

/* Advances the fade record (`+0xb8c`, set up by `UiOptionStartFade`) of the battle-options screen
   `UiOption` by one frame: progress `t` (`+0xb98`) grows by 1/16; the level `+0xb90`
   eases out from the start level `+0xb94` towards +0.8 when opening (`start + (1 - (t-1)^2) * 0.8`)
   or eases in towards 0 when closing (`start - t^2 * 0.8`). When `t` reaches 1 it snaps the level
   to 0.8 (opening) or 0 (closing) and returns 1; otherwise returns 0. */

u8 UiOptionUpdateFade(UiOption *self, u8 closing)
{
    float t = self->fadeT + 0.0625f;
    u8 done = 0;

    if (closing == 0) {
        self->fadeT = t;
        self->fadeLevel = self->fadeStart + (1.0f - (t - 1.0f) * (t - 1.0f)) * 0.8f;
        if (!(t < 1.0f)) {
            self->fadeLevel = 0.8f;
            done = 1;
        }
    } else {
        self->fadeT = t;
        self->fadeLevel = self->fadeStart - t * t * 0.8f;
        if (!(t < 1.0f)) {
            done = 1;
            self->fadeLevel = 0.0f;
        }
    }
    return done;
}

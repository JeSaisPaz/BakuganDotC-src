// bdc 0x089b0ee4 UiBattleModeSelectStepGlow
#include "bdc.h"

/* Blinks the glow overlay (sprite 6+entry) of the enabled entry `entry` of
   `UiBattleModeSelect`: fades its alpha in over 30 frames, then out over
   30 frames, alternating (direction in tween 6+entry's `toggle07`, `+0x16f + entry*0x28`). At each
   end the alpha snaps to 1 or 0 and the ramp restarts from it. Disabled entries are left alone. */

void UiBattleModeSelectStepGlow(UiBattleModeSelect *self, u32 entry)
{
    u32 i = entry & 0xff;
    UiTween *tw;
    GfxSprite *glow;
    float t;
    float start;
    u8 fadingOut;

    if (self->entryEnabled[i] == 0)
        return;

    tw = &self->tweens[i + 6];
    t = tw->t + 0.033333335f;
    fadingOut = tw->toggle07;
    start = tw->startAlpha;
    tw->t = t;
    glow = ((GfxSprite **)self->base.data)[6 + i];
    if (fadingOut == 0) {
        glow->alpha = start + t;
        if (!(tw->t < 1.0f)) {
            ((GfxSprite **)self->base.data)[6 + i]->alpha = 1.0f;
            tw->t = 0.0f;
            tw->startAlpha = ((GfxSprite **)self->base.data)[6 + i]->alpha;
            tw->toggle07 = 1;
        }
    } else {
        glow->alpha = start - t;
        if (!(tw->t < 1.0f)) {
            ((GfxSprite **)self->base.data)[6 + i]->alpha = 0.0f;
            tw->t = 0.0f;
            tw->startAlpha = ((GfxSprite **)self->base.data)[6 + i]->alpha;
            tw->toggle07 = 0;
        }
    }
}

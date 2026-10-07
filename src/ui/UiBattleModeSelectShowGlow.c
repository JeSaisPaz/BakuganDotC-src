// bdc 0x089b0dd4 UiBattleModeSelectShowGlow
#include "bdc.h"

/* Shows or hides the glow overlay of entry `entry` of `UiBattleModeSelect`
   (sprite 6 + entry, `data + 0x18 + entry*4`): hides it when `show == 0` or the entry is disabled
   (`+0x579[entry]`); otherwise makes it visible with centred pivot, scale 1 and alpha 0, and resets
   its blink state (`t` `+0x184`, start alpha `+0x188`, direction `+0x16f` of slot `entry`). */

void UiBattleModeSelectShowGlow(UiBattleModeSelect *self, u8 show, u32 entry)
{
    u32 i = entry & 0xff;
    GfxSprite *glow = ((GfxSprite **)self->base.data)[6 + i];

    if (show == 0) {
        glow->flags &= ~1u;
    } else if (self->entryEnabled[i] == 0) {
        glow->flags &= ~1u;
    } else {
        glow->flags |= 1; /* visible */
        GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[6 + i]);
        ((GfxSprite **)self->base.data)[6 + i]->flags |= 0x20; /* linear filter */
        UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[6 + i], 1.0f, 1.0f, 0.0f);
        ((GfxSprite **)self->base.data)[6 + i]->alpha = 0.0f;
        self->tweens[i + 6].t = 0.0f;
        self->tweens[i + 6].startAlpha = 0.0f;
        self->tweens[i + 6].toggle07 = 0;
    }
}

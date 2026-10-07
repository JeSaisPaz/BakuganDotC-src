// bdc 0x0897168c UiOptionBlinkButtonGlow
#include "bdc.h"

/* When a button row of `UiOption` is selected, pulses the glow sprite 0x34's add
   colour between 0 and 0.5 (±0.0125 per frame, direction `+0x89f`, level `+0x8b8`). */

void UiOptionBlinkButtonGlow(UiOption *self)
{
    float level;
    GfxSprite *glow;

    if ((s8)self->cursor >= 4) {
        level = self->slots[52].pulse.level;
        if (self->slots[52].pulse.waiting == 0) {
            level = level + 0.0125f;
            self->slots[52].pulse.level = level;
            if (!(level < 0.5f)) {
                level = 0.5f;
                self->slots[52].pulse.level = 0.5f;
                self->slots[52].pulse.waiting = 1;
            }
        } else {
            level = level - 0.0125f;
            self->slots[52].pulse.level = level;
            if (level <= 0.0f) {
                self->slots[52].pulse.level = 0.0f;
                level = 0.0f;
                self->slots[52].pulse.waiting = 0;
            }
        }
        level = 0.5f - level;
        glow = ((GfxSprite **)self->base.data)[0x34];
        glow->addColor[0] = level;
        glow->addColor[1] = level;
        glow->addColor[2] = level;
        glow->addColor[3] = 1.0f;
    }
}

// bdc 0x088d2178 UiFieldHudUpdateGaugePanelA
#include "bdc.h"

/* Once story flag 0x38d is set (`GameEventFlagTest`), shows or hides the scan gauge panel of the
   field HUD (task 3001, `UiFieldHudCtor`; sprite array `base.data`) depending on the player's
   `scan` flag, then updates gauge 0 (`UiFieldHudUpdatePowerGauge`) and remembers the flag in
   guideState[4]. Scanning: hides sprites 0, 6, 7, 8, 9, 10, 0x43, shows 0x21 and sets sprite 0x12's
   tint[2] to 0. Not scanning: shows 0, 6, 7, 8, 10, 0x43 and 4, hides 0x21, sets sprite 0x12's
   tint[2] to 1 and resets blinkA. Without the flag nothing happens. */

void UiFieldHudUpdateGaugePanelA(UiFieldHud *self)
{
    u8 scan;
    GfxSprite **sprites;

    if (!GameEventFlagTest(0x38d)) {
        return;
    }
    scan = self->player->scan;
    sprites = (GfxSprite **)self->base.data;
    if (scan != 0) {
        sprites[0]->flags &= ~1u;
        ((GfxSprite **)self->base.data)[6]->flags &= ~1u;
        ((GfxSprite **)self->base.data)[7]->flags &= ~1u;
        ((GfxSprite **)self->base.data)[8]->flags &= ~1u;
        ((GfxSprite **)self->base.data)[9]->flags &= ~1u;
        ((GfxSprite **)self->base.data)[10]->flags &= ~1u;
        ((GfxSprite **)self->base.data)[0x43]->flags &= ~1u;
        ((GfxSprite **)self->base.data)[0x21]->flags |= 1;
        ((GfxSprite **)self->base.data)[0x12]->tint[2] = 0.0f;
    }
    else {
        sprites[0]->flags |= 1;
        ((GfxSprite **)self->base.data)[6]->flags |= 1;
        ((GfxSprite **)self->base.data)[7]->flags |= 1;
        ((GfxSprite **)self->base.data)[8]->flags |= 1;
        ((GfxSprite **)self->base.data)[10]->flags |= 1;
        ((GfxSprite **)self->base.data)[0x43]->flags |= 1;
        ((GfxSprite **)self->base.data)[0x21]->flags &= ~1u;
        ((GfxSprite **)self->base.data)[0x12]->tint[2] = 1.0f;
        ((GfxSprite **)self->base.data)[4]->flags |= 1;
        self->blinkA = 0;
    }
    UiFieldHudUpdatePowerGauge(self, 0, &self->blinkA);
    self->guideState[4] = scan;
}

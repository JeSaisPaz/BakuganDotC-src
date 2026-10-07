// bdc 0x08952f8c UiBattleRuleSelectResetButtonGlow
#include "bdc.h"

/* Resets the glow sprite 5+`index` of rule button `index` of
   `UiBattleRuleSelect`: hidden when `show` is 0 or the button is disabled
   (`+0x5f9 + index`); otherwise visible, centred, linearly filtered, alpha 0, pulse record cleared.
    */

void UiBattleRuleSelectResetButtonGlow(UiBattleRuleSelect *self, u8 show, u8 index)
{
    GfxSprite *glow = ((GfxSprite **)self->base.data)[5 + index];

    if (show == 0) {
        glow->flags &= ~1u;
    } else if (self->buttonEnabled[index] == 0) {
        glow->flags &= ~1u;
    } else {
        glow->flags |= 1; /* visible */
        GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[5 + index]);
        ((GfxSprite **)self->base.data)[5 + index]->flags |= 0x20; /* linear filter */
        UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[5 + index], 1.0f, 1.0f, 0.0f);
        ((GfxSprite **)self->base.data)[5 + index]->alpha = 0.0f;
        self->tweens[index + 5].t = 0.0f;
        self->tweens[index + 5].startAlpha = 0.0f;
        self->tweens[index + 5].toggle07 = 0;
    }
}

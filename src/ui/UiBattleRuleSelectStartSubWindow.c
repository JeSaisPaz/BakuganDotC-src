// bdc 0x089536dc UiBattleRuleSelectStartSubWindow
#include "bdc.h"

/* Starts the open (`closing` = 0: sprite 0 visible, centred, filtered, Y scale 0) or close
   animation of the sub-option window (sprite 0) of `UiBattleRuleSelect`,
   recording t `+0x94`, start alpha `+0x98` and start Y scale `+0x9c`. */

void UiBattleRuleSelectStartSubWindow(UiBattleRuleSelect *self, u8 closing)
{
    GfxSprite *spr = *(GfxSprite **)self->base.data;

    if (closing == 0) {
        spr->layerMask = 2;
        (*(GfxSprite **)self->base.data)->flags |= 1;
        GfxSpriteCenterPivot(*(GfxSprite **)self->base.data);
        (*(GfxSprite **)self->base.data)->flags |= 0x20;
        UiSpriteSetScaleRotation(*(GfxSprite **)self->base.data, 1.0f, 0.0f, 0.0f);
        self->tweens[0].t = 0.0f;
        self->tweens[0].startScale = (*(GfxSprite **)self->base.data)->scaleY;
        self->tweens[0].startAlpha = 0.0f;
    } else {
        self->tweens[0].t = 0.0f;
        self->tweens[0].startScale = spr->scaleX;
        self->tweens[0].startAlpha = 1.0f;
    }
}

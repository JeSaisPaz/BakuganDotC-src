// bdc 0x089723d0 UiOptionAnimateValueChange
#include "bdc.h"

/* Plays the value-change animation of the selected row of the battle-options screen (task 304,
   `maybe_UiScreen304Ctor`): steps `UiFlashStep(0)`; while it runs, dims the row's first value
   sprite (tint 0.3) and grows the row's three value sprites (sprites 1 + cursor*3 + arrowSide*12 ..
   +2) by 0.1 per frame and returns 0; once the flash is done, restores tint 1 and scale 1 and
   returns 1. */

s32 UiOptionAnimateValueChange(UiOption *self)
{
    GfxSprite **sprites;
    GfxSprite *sprite;
    s32 done;
    s32 i;

    done = UiFlashStep(0);
    sprites = (GfxSprite **)self->base.data;
    sprite = sprites[1 + (s8)self->cursor * 3 + self->arrowSide * 12];
    if (done != 0) {
        sprite->tint[0] = 1.0f;
        sprite->tint[1] = 1.0f;
        sprite->tint[2] = 1.0f;
        sprite->alpha = 1.0f;
        for (i = 0; i < 3; i++) {
            sprite = ((GfxSprite **)self->base.data)[1 + i + (s8)self->cursor * 3 + self->arrowSide * 12];
            sprite->scaleX = 1.0f;
            sprite->scaleY = sprite->scaleX;
            sprite->angle = 0.0f;
            GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
        }
        return 1;
    }
    sprite->alpha = 1.0f;
    sprite->tint[0] = 0.3f;
    sprite->tint[1] = 0.3f;
    sprite->tint[2] = 0.3f;
    for (i = 0; i < 3; i++) {
        sprite = ((GfxSprite **)self->base.data)[1 + i + (s8)self->cursor * 3 + self->arrowSide * 12];
        sprite->scaleX = sprite->scaleX + 0.1f;
        sprite->scaleY = sprite->scaleX;
        sprite->angle = 0.0f;
        GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
    }
    return 0;
}

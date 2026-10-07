// bdc 0x08984970 UiCollectionCardAnimatePageArrow
#include "bdc.h"

/* Steps the page-arrow press animation of the card collection screen (task 313): the three
   arrow sprites of side `pageDir` (data sprites 14 + pageDir * 3 + 0..2) stretch every frame
   (scaleX += 0.1, scaleY = 2 * scaleX, angle 0) and the third is tinted 0.3 while flash slot 0
   runs (`UiFlashStep(0)`); once the flash is over the tint goes back to 1, the arrows are reset
   to scale 1 x 2 and it returns 1, else 0. */

s32 UiCollectionCardAnimatePageArrow(UiCollectionCard *self)
{
    s32 done;
    s32 i;
    GfxSprite *sprite;

    done = UiFlashStep(0);
    sprite = ((GfxSprite **)self->base.data)[16 + self->pageDir * 3];
    if (done != 0) {
        sprite->tint[0] = 1.0f;
        sprite->tint[1] = 1.0f;
        sprite->tint[2] = 1.0f;
        sprite->alpha = 1.0f;
        for (i = 0; i < 3; i++) {
            ((GfxSprite **)self->base.data)[14 + self->pageDir * 3 + i]->scaleX = 1.0f;
            ((GfxSprite **)self->base.data)[14 + self->pageDir * 3 + i]->scaleY = 2.0f;
            ((GfxSprite **)self->base.data)[14 + self->pageDir * 3 + i]->angle = 0.0f;
            sprite = ((GfxSprite **)self->base.data)[14 + self->pageDir * 3 + i];
            GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
        }
        return 1;
    }
    sprite->alpha = 1.0f;
    sprite->tint[0] = 0.3f;
    sprite->tint[1] = 0.3f;
    sprite->tint[2] = 0.3f;
    for (i = 0; i < 3; i++) {
        ((GfxSprite **)self->base.data)[14 + self->pageDir * 3 + i]->scaleX += 0.1f;
        sprite = ((GfxSprite **)self->base.data)[14 + self->pageDir * 3 + i];
        sprite->scaleY = sprite->scaleX * 2.0f;
        ((GfxSprite **)self->base.data)[14 + self->pageDir * 3 + i]->angle = 0.0f;
        sprite = ((GfxSprite **)self->base.data)[14 + self->pageDir * 3 + i];
        GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
    }
    return 0;
}

// bdc 0x089b0ab0 UiBattleModeSelectStepArrows
#include "bdc.h"

/* Steps the arrow slide of `UiBattleModeSelect` (sprites/tweens 2..3,
   `t` += 1/8 per frame). Opening: ease-out fade-in (`startAlpha + 1 - (t-1)^2`) and slide from
   `slideStart` by `slideDelta` (arrow 2 to the left, arrow 3 to the right); once `t` reaches 1 it
   snaps alpha to 1, X to `slideEnd` and sets the depth `posZ` to -20. Closing: ease-in fade-out
   (`startAlpha - t^2`), scale `startScale + t^2` and a 64 px outward slide, hiding the arrow once
   `t` reaches 1. Returns true when both arrows have finished this frame. */

bool UiBattleModeSelectStepArrows(UiBattleModeSelect *self, u8 closing)
{
    UiTween *tw;
    GfxSprite *sprite;
    int i;
    u8 done;
    float t;
    float e;

    done = 0;
    if (closing == 0) {
        for (i = 2; i < 4; i++) {
            tw = &self->tweens[i];
            t = tw->t + 0.125f;
            e = t - 1.0f;
            tw->t = t;
            ((GfxSprite **)self->base.data)[i]->alpha = tw->startAlpha + (1.0f - e * e);
            sprite = ((GfxSprite **)self->base.data)[i];
            e = tw->t - 1.0f;
            if (i == 2) {
                sprite->posX = (float)tw->slideStart - (1.0f - e * e) * (float)tw->slideDelta;
            } else {
                sprite->posX = (float)tw->slideStart + (1.0f - e * e) * (float)tw->slideDelta;
            }
            if (!(tw->t < 1.0f)) {
                ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
                ((GfxSprite **)self->base.data)[i]->posX = (float)tw->slideEnd;
                done++;
                ((GfxSprite **)self->base.data)[i]->posZ = -20.0f;
            }
        }
    } else {
        for (i = 2; i < 4; i++) {
            tw = &self->tweens[i];
            t = tw->t + 0.125f;
            tw->t = t;
            ((GfxSprite **)self->base.data)[i]->alpha = tw->startAlpha - t * t;
            ((GfxSprite **)self->base.data)[i]->scaleX = tw->startScale + tw->t * tw->t;
            sprite = ((GfxSprite **)self->base.data)[i];
            sprite->scaleY = sprite->scaleX;
            sprite = ((GfxSprite **)self->base.data)[i];
            GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
            sprite = ((GfxSprite **)self->base.data)[i];
            e = tw->t - 1.0f;
            if (i == 2) {
                sprite->posX = (float)tw->slideStart - (1.0f - e * e) * 64.0f;
            } else {
                sprite->posX = (float)tw->slideStart + (1.0f - e * e) * 64.0f;
            }
            if (!(tw->t < 1.0f)) {
                done++;
                ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
            }
        }
    }
    return done == 2;
}

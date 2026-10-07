// bdc 0x089b08d4 UiBattleModeSelectBeginArrows
#include "bdc.h"

/* Prepares the slide of the two scroll arrows of `UiBattleModeSelect`
   (sprites 2..3, tweens 2..3). Opening (`closing` 0): shows them, centres the pivot, sets linear
   filtering and unit scale, records their home X as the slide end, moves them to the centre
   (x = 240) as the slide start, stores the distance, clears `t` and snapshots the alpha.
   Closing: clears `t` and snapshots alpha, scale X and X for the fade-out. */

void UiBattleModeSelectBeginArrows(UiBattleModeSelect *self, u8 closing)
{
    GfxSprite **sprites;
    UiTween *tw;
    int i;

    if (closing == 0) {
        for (i = 2; i < 4; i++) {
            tw = &self->tweens[i];
            ((GfxSprite **)self->base.data)[i]->flags |= 1;
            GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
            ((GfxSprite **)self->base.data)[i]->flags |= 0x20; /* linear filter */
            UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
            sprites = (GfxSprite **)self->base.data;
            tw->slideEnd = (s16)sprites[i]->posX;
            sprites[i]->posX = 240.0f;
            sprites = (GfxSprite **)self->base.data;
            tw->slideStart = (s16)sprites[i]->posX;
            tw->slideDelta = (s16)UiAbsDiff((float)tw->slideEnd, sprites[i]->posX);
            tw->t = 0.0f;
            tw->startAlpha = ((GfxSprite **)self->base.data)[i]->alpha;
        }
    } else {
        sprites = (GfxSprite **)self->base.data;
        for (i = 2; i < 4; i++) {
            tw = &self->tweens[i];
            tw->t = 0.0f;
            tw->startAlpha = sprites[i]->alpha;
            tw->startScale = sprites[i]->scaleX;
            tw->slideStart = (s16)sprites[i]->posX;
        }
    }
}

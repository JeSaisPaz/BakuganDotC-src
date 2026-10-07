// bdc 0x0893c2ec UiUnlockResultStartPanelTween
#include "bdc.h"

/* Starts the slide/zoom tween of the three panels (data sprites 2..4, `tweens[2..4]`) of the
   unlock-result screen (task 375, `UiUnlockResultCtor`): `out` = 0 sets up the slide-in (sprites
   centred, shrunk to scale 0 and moved 200 px to the side: panel 2 right, panel 4 left), `out` = 1
   the slide-out (the sprite stays put, the target is 200 px to the side). Panel 3 only zooms. */

void UiUnlockResultStartPanelTween(UiUnlockResult *self, bool out)
{
    UiTween *tween;
    GfxSprite *sprite;
    int i;

    if (out) {
        for (i = 2; i < 5; i++) {
            tween = &self->tweens[i];
            tween->t = 0.0f;
            tween->startAlpha = ((GfxSprite **)self->base.data)[i]->alpha;
            tween->startScale = ((GfxSprite **)self->base.data)[i]->scaleX;
            if (i == 2) {
                sprite = ((GfxSprite **)self->base.data)[i];
                tween->slideStart = (s16)(s32)(sprite->posX + 200.0f);
                tween->slideEnd = (s16)(s32)sprite->posX;
                tween->slideDelta =
                    (s16)(s32)UiAbsDiff((float)tween->slideStart, (float)tween->slideEnd);
            } else if (i == 4) {
                sprite = ((GfxSprite **)self->base.data)[i];
                tween->slideStart = (s16)(s32)(sprite->posX - 200.0f);
                tween->slideEnd = (s16)(s32)sprite->posX;
                tween->slideDelta =
                    (s16)(s32)UiAbsDiff((float)tween->slideStart, (float)tween->slideEnd);
            }
        }
    } else {
        for (i = 2; i < 5; i++) {
            tween = &self->tweens[i];
            ((GfxSprite **)self->base.data)[i]->flags |= 1;
            GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
            ((GfxSprite **)self->base.data)[i]->flags |= 0x20;
            UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 0.0f, 0.0f, 0.0f);
            tween->t = 0.0f;
            tween->startAlpha = ((GfxSprite **)self->base.data)[i]->alpha;
            tween->startScale = ((GfxSprite **)self->base.data)[i]->scaleX;
            if (i == 2) {
                sprite = ((GfxSprite **)self->base.data)[i];
                tween->slideStart = (s16)(s32)sprite->posX;
                sprite->posX = sprite->posX + 200.0f;
                tween->slideEnd = (s16)(s32)((GfxSprite **)self->base.data)[i]->posX;
                tween->slideDelta =
                    (s16)(s32)UiAbsDiff((float)tween->slideStart, (float)tween->slideEnd);
            } else if (i == 4) {
                sprite = ((GfxSprite **)self->base.data)[i];
                tween->slideStart = (s16)(s32)sprite->posX;
                sprite->posX = sprite->posX - 200.0f;
                tween->slideEnd = (s16)(s32)((GfxSprite **)self->base.data)[i]->posX;
                tween->slideDelta =
                    (s16)(s32)UiAbsDiff((float)tween->slideStart, (float)tween->slideEnd);
            }
        }
    }
}

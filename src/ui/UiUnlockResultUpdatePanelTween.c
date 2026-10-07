// bdc 0x0893ccdc UiUnlockResultUpdatePanelTween
#include "bdc.h"

/* Advances the panel tween started by `UiUnlockResultStartPanelTween` on the unlock-result screen
   (task 375, `UiUnlockResultCtor`) by 1/8 for the three panels (data sprites 2..4, `tweens[2..4]`)
   and returns true once all three have reached t >= 1 this frame.
   `out` = 0 eases in: alpha and scale grow by 1 - (t - 1)^2, panel 2 moves from `slideEnd` by
   -t^2 * `slideDelta` (panel 4 by +), and a finished panel is snapped to alpha/scale 1 and x =
   `slideStart`. `out` = 1 eases out: alpha and scale shrink by t^2, panel 2 moves by
   +(1 - (t - 1)^2) * `slideDelta` (panel 4 by -), and a finished panel is hidden (flag bit 0
   cleared). Either way the sprite matrix is rebuilt with `GfxSpriteSetScaleRotation`. */

bool UiUnlockResultUpdatePanelTween(UiUnlockResult *self, bool out)
{
    UiTween *tween;
    u8 done;
    int i;

    done = 0;
    if (!out) {
        for (i = 2; i < 5; i++) {
            tween = &self->tweens[i];
            tween->t = tween->t + 0.125f;
            ((GfxSprite **)self->base.data)[i]->alpha =
                tween->startAlpha + (1.0f - (tween->t - 1.0f) * (tween->t - 1.0f));
            ((GfxSprite **)self->base.data)[i]->scaleX =
                tween->startScale + (1.0f - (tween->t - 1.0f) * (tween->t - 1.0f));
            ((GfxSprite **)self->base.data)[i]->scaleY = ((GfxSprite **)self->base.data)[i]->scaleX;
            if (i == 2) {
                ((GfxSprite **)self->base.data)[i]->posX =
                    (float)tween->slideEnd - tween->t * tween->t * (float)tween->slideDelta;
            } else if (i == 4) {
                ((GfxSprite **)self->base.data)[i]->posX =
                    (float)tween->slideEnd + tween->t * tween->t * (float)tween->slideDelta;
            }
            if (!(tween->t < 1.0f)) {
                ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
                ((GfxSprite **)self->base.data)[i]->scaleX = 1.0f;
                ((GfxSprite **)self->base.data)[i]->scaleY = 1.0f;
                if (i == 2 || i == 4) {
                    ((GfxSprite **)self->base.data)[i]->posX = (float)tween->slideStart;
                }
                done++;
            }
            GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i],
                                      ((GfxSprite **)self->base.data)[i]->scaleX,
                                      ((GfxSprite **)self->base.data)[i]->scaleY,
                                      ((GfxSprite **)self->base.data)[i]->angle, false);
        }
    } else {
        for (i = 2; i < 5; i++) {
            tween = &self->tweens[i];
            tween->t = tween->t + 0.125f;
            ((GfxSprite **)self->base.data)[i]->alpha = tween->startAlpha - tween->t * tween->t;
            ((GfxSprite **)self->base.data)[i]->scaleX = tween->startScale - tween->t * tween->t;
            ((GfxSprite **)self->base.data)[i]->scaleY = ((GfxSprite **)self->base.data)[i]->scaleX;
            if (i == 2) {
                ((GfxSprite **)self->base.data)[i]->posX =
                    (float)tween->slideEnd +
                    (1.0f - (tween->t - 1.0f) * (tween->t - 1.0f)) * (float)tween->slideDelta;
            } else if (i == 4) {
                ((GfxSprite **)self->base.data)[i]->posX =
                    (float)tween->slideEnd -
                    (1.0f - (tween->t - 1.0f) * (tween->t - 1.0f)) * (float)tween->slideDelta;
            }
            if (!(tween->t < 1.0f)) {
                ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
                done++;
            }
            GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i],
                                      ((GfxSprite **)self->base.data)[i]->scaleX,
                                      ((GfxSprite **)self->base.data)[i]->scaleY,
                                      ((GfxSprite **)self->base.data)[i]->angle, false);
        }
    }
    return done == 3;
}

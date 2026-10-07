// bdc 0x08939d5c UiUnlockResultPopSprite
#include "bdc.h"

/* Advances the pop animation of sprite `index` of the unlock-result screen (task 375,
   `UiUnlockResultCtor`; reward kind byte `+0x5ee`: 1 card, 2 hologram, 6/9 metal figure, 8
   special unlock, others Maxus parts) by 1/8 per call, using the tween `tweens[index]`
   (`t`, `startAlpha`, `startScale`): `hide == 0` pops it in (alpha `startAlpha + 1 - (t-1)^2`,
   scale `startScale - (1 - (t-1)^2) / 2`, both forced to 1 once done), otherwise grows and fades it
   out (alpha `startAlpha - t^2`, scale `startScale + t^2 / 2`, hidden once done); writes the scale
   with `GfxSpriteSetScaleRotation` and returns true when the animation has finished
   (`!(t < 1)`). */

bool UiUnlockResultPopSprite(UiUnlockResult *self, s8 hide, u8 index)
{
    UiTween *tween = &self->tweens[index];
    float t = tween->t + 0.125f;
    float startAlpha = tween->startAlpha;
    bool done = false;
    GfxSprite *sprite;
    float d;

    if (hide == 0) {
        tween->t = t;
        ((GfxSprite **)self->base.data)[index]->alpha = startAlpha + (1.0f - (t - 1.0f) * (t - 1.0f));
        d = tween->t - 1.0f;
        ((GfxSprite **)self->base.data)[index]->scaleX = tween->startScale - (1.0f - d * d) * 0.5f;
        sprite = ((GfxSprite **)self->base.data)[index];
        sprite->scaleY = sprite->scaleX;
        sprite = ((GfxSprite **)self->base.data)[index];
        if (!(tween->t < 1.0f)) {
            sprite->alpha = 1.0f;
            ((GfxSprite **)self->base.data)[index]->scaleX = 1.0f;
            done = true;
            ((GfxSprite **)self->base.data)[index]->scaleY = 1.0f;
            sprite = ((GfxSprite **)self->base.data)[index];
        }
    } else {
        tween->t = t;
        ((GfxSprite **)self->base.data)[index]->alpha = startAlpha - t * t;
        d = tween->t;
        ((GfxSprite **)self->base.data)[index]->scaleX = tween->startScale + d * d * 0.5f;
        sprite = ((GfxSprite **)self->base.data)[index];
        sprite->scaleY = sprite->scaleX;
        sprite = ((GfxSprite **)self->base.data)[index];
        if (!(tween->t < 1.0f)) {
            sprite->flags &= ~1u;
            done = true;
            sprite = ((GfxSprite **)self->base.data)[index];
        }
    }
    GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
    return done;
}

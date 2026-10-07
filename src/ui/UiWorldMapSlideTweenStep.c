// bdc 0x0899aec4 UiWorldMapSlideTweenStep
#include "bdc.h"

/* One step of the slide tween of sprite `index` of `UiWorldMap` (`spriteTween[index]`):
   `t` += 1/16. Coming in (`out` = 0) with u = t − 1, e = 1 − u²: alpha = startAlpha + e, scale =
   startScale − e/2, X = slideStart − e·slideDelta; once t >= 1 (or NaN) it snaps to alpha 1, scale 1,
   X = slideEnd. Going out: alpha = startAlpha − t², scale = startScale + t²/2, X = slideStart +
   t²·slideDelta, and once t >= 1 the sprite is hidden (flags bit 0 cleared). The sprite matrix is
   rebuilt each step (`GfxSpriteSetScaleRotation`). Returns true when finished. */

bool UiWorldMapSlideTweenStep(UiScreen *screen, u8 out, int index)
{
    UiTween *tween = &((UiWorldMap *)screen)->spriteTween[index];
    float t = tween->t + 0.0625f;
    float startAlpha = tween->startAlpha;
    bool done = false;
    GfxSprite *sprite;
    float u;

    if (out == 0) {
        tween->t = t;
        ((GfxSprite **)screen->data)[index]->alpha = startAlpha + (1.0f - (t - 1.0f) * (t - 1.0f));
        u = tween->t - 1.0f;
        ((GfxSprite **)screen->data)[index]->scaleX = tween->startScale - (1.0f - u * u) * 0.5f;
        sprite = ((GfxSprite **)screen->data)[index];
        sprite->scaleY = sprite->scaleX;
        u = tween->t - 1.0f;
        ((GfxSprite **)screen->data)[index]->posX =
            (float)tween->slideStart - (1.0f - u * u) * (float)tween->slideDelta;
        sprite = ((GfxSprite **)screen->data)[index];
        if (!(tween->t < 1.0f)) {
            sprite->alpha = 1.0f;
            ((GfxSprite **)screen->data)[index]->scaleX = 1.0f;
            ((GfxSprite **)screen->data)[index]->scaleY = 1.0f;
            done = true;
            ((GfxSprite **)screen->data)[index]->posX = (float)tween->slideEnd;
            sprite = ((GfxSprite **)screen->data)[index];
        }
    } else {
        tween->t = t;
        ((GfxSprite **)screen->data)[index]->alpha = startAlpha - t * t;
        ((GfxSprite **)screen->data)[index]->scaleX = tween->startScale + tween->t * tween->t * 0.5f;
        sprite = ((GfxSprite **)screen->data)[index];
        sprite->scaleY = sprite->scaleX;
        ((GfxSprite **)screen->data)[index]->posX =
            (float)tween->slideStart + tween->t * tween->t * (float)tween->slideDelta;
        sprite = ((GfxSprite **)screen->data)[index];
        if (!(tween->t < 1.0f)) {
            sprite->flags &= ~1u;
            done = true;
            sprite = ((GfxSprite **)screen->data)[index];
        }
    }
    GfxSpriteSetScaleRotation(sprite, ((GfxSprite **)screen->data)[index]->scaleX,
                              ((GfxSprite **)screen->data)[index]->scaleY,
                              ((GfxSprite **)screen->data)[index]->angle, false);
    return done;
}

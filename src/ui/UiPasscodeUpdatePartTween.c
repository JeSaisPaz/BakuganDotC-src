// bdc 0x0893df64 UiPasscodeUpdatePartTween
#include "bdc.h"

/* Advances the open/close tween of one part (`part` indexes data sprite `+part*4` and the 0x28-byte
   tween record `tween[part]`) of the sequence-code screen (task 374, `UiPasscodeCtor`; the player
   re-enters a sequence of up to 6 symbols from a 10-symbol pad and it is compared with the answer);
   returns true when that part has finished. */

#define PASSCODE_SPRITE_VISIBLE 1u

bool UiPasscodeUpdatePartTween(UiScreen *screen, bool out, u8 part)
{
    UiPasscode *pc = (UiPasscode *)screen;
    UiPasscodePartTween *tw = &pc->tween[part];
    GfxSprite **sprites = (GfxSprite **)screen->data;
    GfxSprite *sprite;
    bool done = false;
    float t;

    if (out) {
        /* Close: fade and shrink quadratically over 8 steps, then hide. */
        t = tw->t + 0.125f;
        tw->t = t;
        sprites[part]->alpha = tw->startAlpha - t * t;
        sprites[part]->scaleX = tw->startScale - tw->t * tw->t;
        sprites[part]->scaleY = sprites[part]->scaleX;
        sprite = sprites[part];
        if (!(tw->t < 1.0f)) {
            sprite->flags &= ~PASSCODE_SPRITE_VISIBLE;
            done = true;
            sprite = sprites[part];
        }
    } else if (tw->flag == 0) {
        /* Open, first stage: ease alpha in and over-scale towards 1.2. */
        t = tw->t + 0.125f;
        tw->t = t;
        sprites[part]->alpha = tw->startAlpha + (1.0f - (t - 1.0f) * (t - 1.0f));
        sprites[part]->scaleX =
            tw->startScale + (1.0f - (tw->t - 1.0f) * (tw->t - 1.0f)) * 1.2f;
        sprites[part]->scaleY = sprites[part]->scaleX;
        sprite = sprites[part];
        if (!(tw->t < 1.0f)) {
            sprite->alpha = 1.0f;
            tw->flag = 1;
            tw->t = 0.0f;
            tw->startScale = sprites[part]->scaleX;
            sprite = sprites[part];
        }
    } else {
        /* Open, second stage: settle the scale back down to 1.0. */
        t = tw->t + 0.25f;
        tw->t = t;
        sprites[part]->scaleX = tw->startScale - t * t * 0.2f;
        sprites[part]->scaleY = sprites[part]->scaleX;
        sprite = sprites[part];
        if (!(tw->t < 1.0f)) {
            sprite->alpha = 1.0f;
            sprites[part]->scaleX = 1.0f;
            sprites[part]->scaleY = 1.0f;
            done = true;
            sprite = sprites[part];
        }
    }
    GfxSpriteSetScaleRotation(sprite, sprites[part]->scaleX, sprites[part]->scaleY,
                              sprites[part]->angle, false);
    return done;
}

// bdc 0x0895465c UiBattleRuleSelectUpdateButtonTween
#include "bdc.h"

/* Advances the button tweens of the battle-rule menu (task 340, `UiBattleRuleSelectCtor`; four
   battle types, reached from the battle-mode screen 350; the class purpose is inferred from what
   it writes to the save profile) set up by `UiBattleRuleSelectStartButtonTween`.
   Slide-in (`out` = 0): for the four buttons (sprites/tweens 1..4), after the tween delay, the
   first leg eases X from `slideStart` by `t^2 * slideDelta` (t += 1/8); on arrival X = 704, unit
   scale, then a second leg (delay 4) eases back with `1 - (t-1)^2` (t += 1/16) to `panelHomeX` for
   the cursor button or -224 for the others, and finally tints the button (grey 0.6 if disabled,
   white otherwise, alpha 1). Returns true once all four buttons have finished the second
   leg (finished buttons keep t >= 1 and are counted on every call).
   Slide-out: fades the cursor button's alpha by t^2 and grows its scale by t^2 (t += 1/8); when
   t >= 1 hides it and returns true, else false. */

bool UiBattleRuleSelectUpdateButtonTween(UiBattleRuleSelect *self, bool out)
{
    GfxSprite *sprite;
    UiTween *tw;
    s8 arrived;
    int i;
    int cur;
    float t;
    float start;
    float delta;
    float u;

    arrived = 0;
    if (!out) {
        tw = &self->tweens[1];
        for (i = 1; i < 5; i++, tw++) {
            if (tw->delay0b != 0) {
                tw->delay0b--;
                continue;
            }
            t = tw->t;
            start = (float)tw->slideStart;
            delta = (float)tw->slideDelta;
            if (tw->toggle07 == 0) {
                t = t + 0.125f;
                tw->t = t;
                ((GfxSprite **)self->base.data)[i]->posX = start + t * t * delta;
                if (!(tw->t < 1.0f)) {
                    ((GfxSprite **)self->base.data)[i]->posX = 704.0f;
                    ((GfxSprite **)self->base.data)[i]->scaleX = 1.0f;
                    ((GfxSprite **)self->base.data)[i]->scaleY = 1.0f;
                    sprite = ((GfxSprite **)self->base.data)[i];
                    GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
                    tw->toggle07 = 1;
                    tw->t = 0.0f;
                    sprite = ((GfxSprite **)self->base.data)[i];
                    tw->delay0b = 4;
                    tw->slideStart = (s16)(int)sprite->posX;
                    if (self->cursor == i - 1) {
                        tw->slideDelta = (s16)(int)(sprite->posX - self->panelHomeX);
                    } else {
                        tw->slideDelta = (s16)(int)(sprite->posX - -224.0f);
                    }
                }
            } else {
                t = t + 0.0625f;
                u = t - 1.0f;
                tw->t = t;
                ((GfxSprite **)self->base.data)[i]->posX = start - (1.0f - u * u) * delta;
                if (!(tw->t < 1.0f)) {
                    if (self->cursor == i - 1) {
                        ((GfxSprite **)self->base.data)[i]->posX = self->panelHomeX;
                    } else {
                        ((GfxSprite **)self->base.data)[i]->posX = -224.0f;
                    }
                    sprite = ((GfxSprite **)self->base.data)[i];
                    if (self->buttonEnabled[i - 1] == 0) {
                        sprite->tint[0] = 0.6f;
                        sprite->tint[1] = 0.6f;
                        sprite->tint[2] = 0.6f;
                        sprite->alpha = 1.0f;
                    } else {
                        sprite->tint[0] = 1.0f;
                        sprite->tint[1] = 1.0f;
                        sprite->tint[2] = 1.0f;
                        sprite->alpha = 1.0f;
                    }
                    arrived++;
                }
            }
        }
        return arrived == 4;
    }

    cur = self->cursor;
    tw = &self->tweens[cur + 1];
    start = tw->startAlpha;
    t = tw->t + 0.125f;
    tw->t = t;
    ((GfxSprite **)self->base.data)[cur + 1]->alpha = start - t * t;
    cur = self->cursor;
    t = self->tweens[cur + 1].t;
    ((GfxSprite **)self->base.data)[cur + 1]->scaleX = self->tweens[cur + 1].startScale + t * t;
    sprite = ((GfxSprite **)self->base.data)[self->cursor + 1];
    sprite->scaleY = sprite->scaleX;
    sprite = ((GfxSprite **)self->base.data)[self->cursor + 1];
    GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
    if (!(self->tweens[self->cursor + 1].t < 1.0f)) {
        ((GfxSprite **)self->base.data)[self->cursor + 1]->flags &= ~1u;
        return true;
    }
    return false;
}

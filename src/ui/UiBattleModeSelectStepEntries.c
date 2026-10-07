// bdc 0x089b0410 UiBattleModeSelectStepEntries
#include "bdc.h"

/* Steps the entry-panel animation of `UiBattleModeSelect` (panels =
   sprites 4..5, per-panel state `tweens[4..5]`). Opening: after its delay each panel accelerates
   (`t^2`, 1/8 per frame) across to x = 704, then eases back (1/16 per frame) to its home (`+0x57c`)
   when it is the selected entry or to x = -224 otherwise, ending with its enabled/disabled tint;
   returns true when both panels are done. Closing: fades the selected panel out (`alpha - t^2`)
   while growing it (`scale + t^2`), hides it and returns true at `t >= 1`. */

bool UiBattleModeSelectStepEntries(UiBattleModeSelect *self, u8 closing)
{
  GfxSprite *sprite;
  UiTween *tween;
  int i;
  int cur;
  u8 done;
  u8 enabled;
  float start;
  float delta;
  float t;
  float u;

  done = 0;
  if (closing == 0) {
    for (i = 4; i < 6; i++) {
      tween = &self->tweens[i];
      if (tween->delay0b != 0) {
        tween->delay0b--;
        continue;
      }
      start = (float)tween->slideStart;
      delta = (float)tween->slideDelta;
      if (tween->toggle07 == 0) {
        /* phase 1: accelerate across to x = 704 */
        t = tween->t + 0.125f;
        tween->t = t;
        ((GfxSprite **)self->base.data)[i]->posX = start + t * t * delta;
        if (tween->t < 1.0f) {
          continue;
        }
        ((GfxSprite **)self->base.data)[i]->posX = 704.0f;
        ((GfxSprite **)self->base.data)[i]->scaleX = 1.0f;
        ((GfxSprite **)self->base.data)[i]->scaleY = 1.0f;
        sprite = ((GfxSprite **)self->base.data)[i];
        GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
        tween->toggle07 = 1;
        tween->t = 0.0f;
        sprite = ((GfxSprite **)self->base.data)[i];
        tween->delay0b = 4;
        tween->slideStart = (s16)(int)sprite->posX;
        if (self->cursor == i - 4) {
          tween->slideDelta = (s16)(int)(sprite->posX - self->panelHomeX);
        } else {
          tween->slideDelta = (s16)(int)(sprite->posX - -224.0f);
        }
      } else {
        /* phase 2: ease back to the home / off-screen x */
        t = tween->t + 0.0625f;
        u = t - 1.0f;
        tween->t = t;
        ((GfxSprite **)self->base.data)[i]->posX = start - (1.0f - u * u) * delta;
        if (tween->t < 1.0f) {
          continue;
        }
        if (self->cursor == i - 4) {
          ((GfxSprite **)self->base.data)[i]->posX = self->panelHomeX;
        } else {
          ((GfxSprite **)self->base.data)[i]->posX = -224.0f;
        }
        enabled = self->entryEnabled[i - 4];
        sprite = ((GfxSprite **)self->base.data)[i];
        if (enabled != 0) {
          sprite->tint[0] = 1.0f;
          sprite->tint[1] = 1.0f;
          sprite->tint[2] = 1.0f;
          sprite->alpha = 1.0f;
        } else {
          sprite->tint[0] = 0.6f;
          sprite->tint[1] = 0.6f;
          sprite->tint[2] = 0.6f;
          sprite->alpha = 1.0f;
        }
        done++;
      }
    }
    return done == 2;
  }

  cur = self->cursor;
  tween = &self->tweens[cur + 4];
  t = tween->t + 0.125f;
  tween->t = t;
  ((GfxSprite **)self->base.data)[cur + 4]->alpha = tween->startAlpha - t * t;
  cur = self->cursor;
  t = self->tweens[cur + 4].t;
  ((GfxSprite **)self->base.data)[cur + 4]->scaleX = self->tweens[cur + 4].startScale + t * t;
  sprite = ((GfxSprite **)self->base.data)[self->cursor + 4];
  sprite->scaleY = sprite->scaleX;
  sprite = ((GfxSprite **)self->base.data)[self->cursor + 4];
  GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
  cur = self->cursor;
  if (self->tweens[cur + 4].t < 1.0f) {
    return false;
  }
  ((GfxSprite **)self->base.data)[cur + 4]->flags &= ~1u;
  return true;
}

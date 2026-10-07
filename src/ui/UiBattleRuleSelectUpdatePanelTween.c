// bdc 0x08954b20 UiBattleRuleSelectUpdatePanelTween
#include "bdc.h"

/* Advances the slide/fade of the two side panels (data sprites 9/10 at `+0x24/+0x28`, records
   `tweens[9..10]`) of the battle-rule menu (task 340, `UiBattleRuleSelectCtor`; four battle
   types, reached from the battle-mode screen 350; the class purpose is inferred from what it writes
   to the save profile); returns true when both are done. Panel 9 slides left, panel 10 right.
   In (`out` false): t += 1/8, alpha rises by an ease-out from the snapshot, the panel slides by
   `slideDelta`; when done it is pinned at alpha 1, x = `slideEnd`, z = -20. Out: alpha falls and
   scale grows by t^2, the panel slides 64 px outward; when done it is hidden (flags bit 0). */

bool UiBattleRuleSelectUpdatePanelTween(UiBattleRuleSelect *self, bool out)
{
  GfxSprite *sprite;
  UiTween *tween;
  int i;
  u8 done;
  float t;
  float u;

  done = 0;
  if (!out) {
    for (i = 9; i < 11; i++) {
      tween = &self->tweens[i];
      t = tween->t + 0.125f;
      u = t - 1.0f;
      tween->t = t;
      ((GfxSprite **)self->base.data)[i]->alpha = tween->startAlpha + (1.0f - u * u);
      sprite = ((GfxSprite **)self->base.data)[i];
      u = tween->t - 1.0f;
      if (i == 9) {
        sprite->posX = (float)tween->slideStart - (1.0f - u * u) * (float)tween->slideDelta;
      } else {
        sprite->posX = (float)tween->slideStart + (1.0f - u * u) * (float)tween->slideDelta;
      }
      if (!(tween->t < 1.0f)) {
        ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
        ((GfxSprite **)self->base.data)[i]->posX = (float)tween->slideEnd;
        done++;
        ((GfxSprite **)self->base.data)[i]->posZ = -20.0f;
      }
    }
  } else {
    for (i = 9; i < 11; i++) {
      tween = &self->tweens[i];
      t = tween->t + 0.125f;
      tween->t = t;
      ((GfxSprite **)self->base.data)[i]->alpha = tween->startAlpha - t * t;
      ((GfxSprite **)self->base.data)[i]->scaleX = tween->startScale + tween->t * tween->t;
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->scaleY = sprite->scaleX;
      sprite = ((GfxSprite **)self->base.data)[i];
      GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
      sprite = ((GfxSprite **)self->base.data)[i];
      u = tween->t - 1.0f;
      if (i == 9) {
        sprite->posX = (float)tween->slideStart - (1.0f - u * u) * 64.0f;
      } else {
        sprite->posX = (float)tween->slideStart + (1.0f - u * u) * 64.0f;
      }
      if (!(tween->t < 1.0f)) {
        done++;
        ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
      }
    }
  }
  return done == 2;
}

// bdc 0x089537b0 UiBattleRuleSelectUpdateSubWindow
#include "bdc.h"

/* Advances the sub-option window animation of `UiBattleRuleSelect` by 1/8
   (tween 0 `t`): opening sets sprite 0's alpha to `startAlpha + (1 - (t-1)²) * 0.8` and its Y scale
   to `startScale + (1 - (t-1)²)`, snapping to alpha 0.8 / scale 1 once `t` reaches 1; closing sets
   alpha `startAlpha - t² * 0.8` and Y scale `startScale - t²`, hiding the sprite (flags bit0) once
   `t` reaches 1. Reapplies the sprite's scale/rotation either way. Returns true when `t` >= 1
   (finished), false otherwise. */

bool UiBattleRuleSelectUpdateSubWindow(UiBattleRuleSelect *self, u8 closing)
{
  float t = self->tweens[0].t + 0.125f;
  float startAlpha = self->tweens[0].startAlpha;
  bool done = false;
  GfxSprite *sprite;
  float d;

  if (closing == 0) {
    d = t - 1.0f;
    self->tweens[0].t = t;
    ((GfxSprite **)self->base.data)[0]->alpha = startAlpha + (1.0f - d * d) * 0.8f;
    d = self->tweens[0].t - 1.0f;
    ((GfxSprite **)self->base.data)[0]->scaleY = self->tweens[0].startScale + (1.0f - d * d);
    sprite = ((GfxSprite **)self->base.data)[0];
    if (!(self->tweens[0].t < 1.0f)) {
      sprite->alpha = 0.8f;
      done = true;
      ((GfxSprite **)self->base.data)[0]->scaleY = 1.0f;
      sprite = ((GfxSprite **)self->base.data)[0];
    }
  } else {
    self->tweens[0].t = t;
    ((GfxSprite **)self->base.data)[0]->alpha = startAlpha - t * t * 0.8f;
    t = self->tweens[0].t;
    ((GfxSprite **)self->base.data)[0]->scaleY = self->tweens[0].startScale - t * t;
    sprite = ((GfxSprite **)self->base.data)[0];
    if (!(self->tweens[0].t < 1.0f)) {
      sprite->flags &= ~1u;
      done = true;
      sprite = ((GfxSprite **)self->base.data)[0];
    }
  }
  GfxSpriteSetScaleRotation(sprite, ((GfxSprite **)self->base.data)[0]->scaleX,
                            ((GfxSprite **)self->base.data)[0]->scaleY,
                            ((GfxSprite **)self->base.data)[0]->angle, false);
  return done;
}

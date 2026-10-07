// bdc 0x089ae4a0 UiPauseSettingsStepArrows
#include "bdc.h"

/* Steps the arrow slide of `UiPauseSettings` prepared by
   `UiPauseSettingsBeginArrows` at 1/16 per frame (sprites 1..24, state `arrowSlides[i-1]`).
   Opening: ease-out (`1 - (t-1)^2`) alpha and X slide toward the centre (left arrows 1..12 +,
   right arrows 13..24 -), snapped to alpha 1 and `endX` once `t >= 1`. Closing: ease-in (`t^2`)
   fade-out, X slide and +50 % scale, hiding each sprite once `t >= 1`. Returns true when at least
   one arrow reached `t >= 1` this frame (all run in lockstep). */

bool UiPauseSettingsStepArrows(UiPauseSettings *self, u8 closing)
{
  UiArrowSlide *slide;
  GfxSprite *sprite;
  u8 finished;
  int i;
  float t;
  float ease;

  finished = 0;
  if (closing == 0) {
    for (i = 1; i < 25; i++) {
      slide = &self->arrowSlides[i - 1];
      t = slide->t + 0.0625f;
      slide->t = t;
      ((GfxSprite **)self->base.data)[i]->alpha = slide->startAlpha + (1.0f - (t - 1.0f) * (t - 1.0f));
      sprite = ((GfxSprite **)self->base.data)[i];
      ease = 1.0f - (slide->t - 1.0f) * (slide->t - 1.0f);
      if (i < 13) {
        sprite->posX = (float)slide->startX + ease * (float)slide->distX;
      } else {
        sprite->posX = (float)slide->startX - ease * (float)slide->distX;
      }
      if (!(slide->t < 1.0f)) {
        ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
        finished++;
        ((GfxSprite **)self->base.data)[i]->posX = (float)slide->endX;
      }
    }
  } else {
    for (i = 1; i < 25; i++) {
      slide = &self->arrowSlides[i - 1];
      t = slide->t + 0.0625f;
      slide->t = t;
      ((GfxSprite **)self->base.data)[i]->alpha = slide->startAlpha - t * t;
      ((GfxSprite **)self->base.data)[i]->posX =
          (float)slide->startX + slide->t * slide->t * (float)slide->distX;
      ((GfxSprite **)self->base.data)[i]->scaleX = slide->startScale + slide->t * slide->t * 0.5f;
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->scaleY = sprite->scaleX;
      sprite = ((GfxSprite **)self->base.data)[i];
      GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
      if (!(slide->t < 1.0f)) {
        finished++;
        sprite = ((GfxSprite **)self->base.data)[i];
        sprite->flags &= ~1u;
      }
    }
  }
  return finished != 0;
}

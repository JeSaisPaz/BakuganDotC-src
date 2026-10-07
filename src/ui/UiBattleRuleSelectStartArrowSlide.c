// bdc 0x08952db0 UiBattleRuleSelectStartArrowSlide
#include "bdc.h"

/* Starts the slide of the two arrow sprites 9–10 of `UiBattleRuleSelect`
   (tweens 9–10): opening makes them visible, centred, linearly filtered and moves them from
   X = 240 to their layout X (distance via `UiAbsDiff`); closing records the current
   alpha/scale/X. */

void UiBattleRuleSelectStartArrowSlide(UiBattleRuleSelect *self, u8 closing)
{
  UiTween *tw;
  GfxSprite **sprites;
  float dist;
  int i;

  if (closing == 0) {
    tw = &self->tweens[9];
    for (i = 9; i < 11; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1; /* visible */
      GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
      ((GfxSprite **)self->base.data)[i]->flags |= 0x20; /* linear filter */
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
      sprites = (GfxSprite **)self->base.data;
      tw->slideEnd = (s16)sprites[i]->posX;
      sprites[i]->posX = 240.0f;
      sprites = (GfxSprite **)self->base.data;
      tw->slideStart = (s16)sprites[i]->posX;
      dist = UiAbsDiff((float)tw->slideEnd, sprites[i]->posX);
      tw->t = 0.0f;
      tw->slideDelta = (s16)dist;
      tw->startAlpha = ((GfxSprite **)self->base.data)[i]->alpha;
      tw++;
    }
  } else {
    sprites = (GfxSprite **)self->base.data;
    tw = &self->tweens[9];
    for (i = 9; i < 11; i++) {
      tw->t = 0.0f;
      tw->startAlpha = sprites[i]->alpha;
      tw->startScale = sprites[i]->scaleX;
      tw->slideStart = (s16)sprites[i]->posX;
      tw++;
    }
  }
}

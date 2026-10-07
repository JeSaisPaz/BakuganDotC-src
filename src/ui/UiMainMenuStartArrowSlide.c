// bdc 0x089a8734 UiMainMenuStartArrowSlide
#include "bdc.h"

/* Starts the slide-in (`closing` 0: from x = 240 to each sprite's layout x) or slide-out of the two
   arrow sprites (layout sprites 3 and 4, `data+0xc`/`+0x10`), recording start/end/distance and the
   start alpha in tween slots 3 and 4. Stepped by `UiMainMenuStepArrowSlide`. */

void UiMainMenuStartArrowSlide(UiMainMenu *self, u8 closing)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  GfxSprite *sprite;
  UiTween *tween;
  int i;

  if (closing == 0) {
    for (i = 3; i < 5; i++) {
      tween = &self->slots[i].tween;
      sprites[i]->flags |= 1;
      GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
      ((GfxSprite **)self->base.data)[i]->flags |= 0x20;
      ((GfxSprite **)self->base.data)[i]->scaleX = 1.0f;
      ((GfxSprite **)self->base.data)[i]->scaleY = 1.0f;
      ((GfxSprite **)self->base.data)[i]->angle = 0.0f;
      sprite = ((GfxSprite **)self->base.data)[i];
      GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
      sprites = (GfxSprite **)self->base.data;
      tween->slideEnd = (s16)sprites[i]->posX;
      sprites[i]->posX = 240.0f;
      sprites = (GfxSprite **)self->base.data;
      tween->slideStart = (s16)sprites[i]->posX;
      tween->slideDelta = (s16)UiAbsDiff((float)tween->slideEnd, sprites[i]->posX);
      tween->t = 0.0f;
      tween->startAlpha = sprites[i]->alpha;
    }
  } else {
    for (i = 3; i < 5; i++) {
      tween = &self->slots[i].tween;
      tween->t = 0.0f;
      tween->startAlpha = sprites[i]->alpha;
      tween->startScale = sprites[i]->scaleX;
      tween->slideStart = (s16)sprites[i]->posX;
    }
  }
}

// bdc 0x089ae19c UiPauseSettingsBeginArrows
#include "bdc.h"

/* Prepares the slide of the 24 left/right arrow sprites of `UiPauseSettings`
   (sprites 1..24 of `data`: 1..12 left arrows, 13..24 right arrows, state `arrowSlides[i-1]`).
   Opening: tints every third sprite blue (0, 0.5, 1) with alpha 0, shows them, centres the pivot
   with linear filtering, resets the scale/angle, records the current X as `endX`, offsets the left
   arrows by -16 px and the right arrows by +16 px (right ones are mirrored, `GfxSpriteFlipU`) and
   records `startX`, `distX` (`UiAbsDiff`), `t` 0 and alpha 0. Closing: records a +32 px slide from
   the current X and start alpha/scale 1.0; the sprites themselves are left untouched. */

void UiPauseSettingsBeginArrows(UiPauseSettings *self, u8 closing)
{
  UiArrowSlide *slide;
  GfxSprite *sprite;
  int i;
  float endX;

  if (closing == 0) {
    for (i = 1; i < 25; i++) {
      slide = &self->arrowSlides[i - 1];
      sprite = ((GfxSprite **)self->base.data)[i];
      switch (i) {
      case 3: case 6: case 9: case 12: case 15: case 18: case 21: case 24:
        sprite->tint[0] = 0.0f;
        sprite->tint[1] = 0.5f;
        sprite->tint[2] = 1.0f;
        sprite->alpha = 0.0f;
        sprite = ((GfxSprite **)self->base.data)[i];
        break;
      default:
        break;
      }
      sprite->flags |= 1;
      GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
      ((GfxSprite **)self->base.data)[i]->flags |= 0x20;
      ((GfxSprite **)self->base.data)[i]->scaleX = 1.0f;
      ((GfxSprite **)self->base.data)[i]->scaleY = 1.0f;
      ((GfxSprite **)self->base.data)[i]->angle = 0.0f;
      sprite = ((GfxSprite **)self->base.data)[i];
      GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
      slide->endX = (s16)(int)((GfxSprite **)self->base.data)[i]->posX;
      if (i < 13) {
        ((GfxSprite **)self->base.data)[i]->posX -= 16.0f;
      } else {
        ((GfxSprite **)self->base.data)[i]->posX += 16.0f;
        GfxSpriteFlipU(((GfxSprite **)self->base.data)[i]);
      }
      endX = (float)slide->endX;
      sprite = ((GfxSprite **)self->base.data)[i];
      slide->startX = (s16)(int)sprite->posX;
      slide->distX = (s16)(int)UiAbsDiff(sprite->posX, endX);
      slide->t = 0.0f;
      slide->startAlpha = 0.0f;
      ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
    }
  } else {
    for (i = 1; i < 25; i++) {
      slide = &self->arrowSlides[i - 1];
      sprite = ((GfxSprite **)self->base.data)[i];
      slide->endX = (s16)(int)(sprite->posX + 32.0f);
      slide->startX = (s16)(int)sprite->posX;
      slide->distX = (s16)(int)UiAbsDiff(sprite->posX, (float)slide->endX);
      slide->t = 0.0f;
      slide->startAlpha = 1.0f;
      slide->startScale = 1.0f;
    }
  }
}

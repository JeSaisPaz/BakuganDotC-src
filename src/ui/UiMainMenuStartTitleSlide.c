// bdc 0x089a73fc UiMainMenuStartTitleSlide
#include "bdc.h"

/* Starts the vertical slide of the title sprite (`data+0x18`): from its current y to y+20 (`dir` 0)
   or y-20 (`dir` 1), clearing the slide state in `slots[6]` and snapshotting the sprite's scale.
   Stepped by `UiMainMenuStepTitleSlide`. */

void UiMainMenuStartTitleSlide(UiMainMenu *self, u8 dir)
{
  GfxSprite **sprites;
  UiTween *tween = &self->slots[6].tween;

  memset(tween, 0, sizeof(UiTween));
  sprites = (GfxSprite **)self->base.data;
  if (dir == 0) {
    tween->slideEnd = (s16)(int)(sprites[6]->posY + 20.0f);
  } else {
    tween->slideEnd = (s16)(int)(sprites[6]->posY - 20.0f);
  }
  tween->slideStart = (s16)(int)sprites[6]->posY;
  tween->slideDelta = (s16)(int)UiAbsDiff((float)tween->slideEnd, (float)tween->slideStart);
  tween->startScale = sprites[6]->scaleX;
}

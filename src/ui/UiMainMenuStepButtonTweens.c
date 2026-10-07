// bdc 0x089a949c UiMainMenuStepButtonTweens
#include "bdc.h"

/* Steps the four button-sprite tweens (slots 15..18, `UiTweenUpdate`, 8 frames); returns true once
   any of them has finished (UiTweenUpdate returns true when its tween is done). */

int UiMainMenuStepButtonTweens(UiMainMenu *self, u8 closing)

{
  u8 finished;
  int i;

  finished = 0;
  for (i = 15; i < 19; i++) {
    /* self->base.data is re-read every iteration, after the previous call */
    finished += UiTweenUpdate(1.0f, 1.0f, 8.0f, closing, ((GfxSprite **)self->base.data)[i],
                              &self->slots[i].tween, 1);
  }
  return finished != 0;
}

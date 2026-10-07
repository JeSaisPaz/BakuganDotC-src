// bdc 0x0898f428 UiCollectionFigureMotionButtonsDone
#include "bdc.h"

/* Advances the fade tweens of the second-detail-view button sprites 0x38..0x3a and 0x3c..0x3e
   (records `+0x934`, `+0x9d4`) of `UiCollectionFigure` started by
   `UiCollectionFigureStartMotionButtonsTween`; `out` selects fade-out. Returns true when at
   least one of the six `UiTweenUpdate` calls reported its transition finished (they run in
   lockstep, so: when done). */

bool UiCollectionFigureMotionButtonsDone(UiCollectionFigure *self, u8 out)
{
  GfxSprite **sprites;
  u8 finished = 0;
  s32 i;

  for (i = 0x38; i < 0x3b; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x3c; i < 0x3f; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, sprites[i], &self->tweens[i], 1);
  }
  return finished != 0;
}

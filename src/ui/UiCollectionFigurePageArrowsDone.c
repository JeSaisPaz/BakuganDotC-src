// bdc 0x0898d1e4 UiCollectionFigurePageArrowsDone
#include "bdc.h"

/* Advances the fade tweens of the page-arrow sprites 0x13..0x18 (records from `+0x36c`) of
   `UiCollectionFigure` started by
   `UiCollectionFigureStartPageArrowTween`; returns true when finished. */

bool UiCollectionFigurePageArrowsDone(UiCollectionFigure *self, u8 out)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  u8 active = 0;
  s32 i;

  for (i = 0x13; i < 0x19; i++) {
    active += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, sprites[i], &self->tweens[i], 1);
  }
  return active != 0;
}

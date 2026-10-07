// bdc 0x0892e40c UiBakuganSelectGridDone
#include "bdc.h"

/* Advances the grid tweens (sprites 0x1a..0x55 and 0x5e..0x71, flags 5) started by
   `UiBakuganSelectTweenGrid` by one frame; returns true once any of them has
   finished (UiTweenUpdate returns true when its tween is done). */

bool UiBakuganSelectGridDone(UiBakuganSelect *self, u8 hide)
{
  u8 finished = 0;
  int i;

  for (i = 0x1a; i < 0x2e; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 5);
  }
  for (i = 0x2e; i < 0x42; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 5);
  }
  for (i = 0x42; i < 0x56; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 5);
  }
  for (i = 0x5e; i < 0x72; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 5);
  }
  return finished != 0;
}

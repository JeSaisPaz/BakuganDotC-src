// bdc 0x0892f7f8 UiBakuganSelectTypeIconsDone
#include "bdc.h"

/* Advances the attribute icon tweens (sprites/tweens 0x7e..0x83) of `UiBakuganSelectTweenTypeIcons`
   and returns true once any of them has finished (UiTweenUpdate returns true when a tween is done). */

bool UiBakuganSelectTypeIconsDone(UiBakuganSelect *self, u8 hide)

{
  u8 finished = 0;
  int i;

  for (i = 0x7e; i < 0x84; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 5);
  }
  return finished != 0;
}

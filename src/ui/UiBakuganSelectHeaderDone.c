// bdc 0x0892e08c UiBakuganSelectHeaderDone
#include "bdc.h"

/* Advances the header tweens (sprites 2..5, 8..11 and 12..15, flags 9) started by
   `UiBakuganSelectTweenHeaderSprites` and returns true once any of them has finished
   (UiTweenUpdate returns true when its tween is done; the u8 sum is tested against 0). */

bool UiBakuganSelectHeaderDone(UiBakuganSelect *self, u8 hide)
{
  u8 finished = 0;
  int i;

  for (i = 2; i < 6; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 9);
  }
  for (i = 8; i < 12; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 9);
  }
  for (i = 12; i < 16; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 9);
  }
  return finished != 0;
}

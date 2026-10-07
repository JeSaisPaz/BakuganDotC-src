// bdc 0x0892dc0c UiBakuganSelectStatBarsADone
#include "bdc.h"

/* Advances the tweens of `UiBakuganSelectTweenStatBarsA` (sprites/tweens 0x56 and 0x58)
 * by one frame and returns true once at least one of them has finished
 * (UiTweenUpdate returns true when done). */
bool UiBakuganSelectStatBarsADone(UiBakuganSelect *self, u8 hide)
{
  u8 finished = 0;
  int i;

  for (i = 0x56; i < 0x57; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 1);
  for (i = 0x58; i < 0x59; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 1);
  return finished != 0;
}

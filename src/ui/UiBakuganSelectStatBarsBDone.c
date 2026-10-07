// bdc 0x0892df70 UiBakuganSelectStatBarsBDone
#include "bdc.h"

/* Advances the tweens of `UiBakuganSelectTweenStatBarsB` (sprites/tweens 0x57 and 0x59)
 * one frame and returns true once any of them reports finished (UiTweenUpdate returns
 * true when done). */
bool UiBakuganSelectStatBarsBDone(UiBakuganSelect *self, u8 hide)
{
  u8 finished = 0;
  int i;

  for (i = 0x57; i < 0x58; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 1);
  for (i = 0x59; i < 0x5a; i++)
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 1);
  return finished != 0;
}

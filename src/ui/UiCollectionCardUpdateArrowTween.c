// bdc 0x0898401c UiCollectionCardUpdateArrowTween
#include "bdc.h"

/* Advances the six page arrow tweens (tweens/sprites 17..22) of
   `UiCollectionCard` (`UiTweenUpdate` mode 1); returns true once ANY of
   them has finished (u8 sum of the finished flags is non-zero). */

bool UiCollectionCardUpdateArrowTween(UiCollectionCard *self, u8 out)
{
  u8 finished = 0;
  int i;

  for (i = 0x11; i < 0x17; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
  }
  return finished != 0;
}

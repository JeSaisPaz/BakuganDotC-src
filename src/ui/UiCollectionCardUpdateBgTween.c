// bdc 0x089830cc UiCollectionCardUpdateBgTween
#include "bdc.h"

/* Advances the tweens of background sprites 0x1f..0x23 of `UiCollectionCard`
   (`UiTweenUpdate` flags 1, 16 frames); returns true once any of them has finished
   (u8 sum of the per-tween "finished" results is non-zero). */

bool UiCollectionCardUpdateBgTween(UiCollectionCard *self, u8 out)

{
  u8 finished;
  int i;

  finished = 0;
  for (i = 0x1f; i < 0x24; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
  }
  return finished != 0;
}

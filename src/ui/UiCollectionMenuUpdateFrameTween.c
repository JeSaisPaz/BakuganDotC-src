// bdc 0x089753bc UiCollectionMenuUpdateFrameTween
#include "bdc.h"

/* Advances the five frame sprite tweens (slots 20..24) of `UiCollectionMenu`
   (`UiTweenUpdate` mode 1); returns true once any of them has finished (the count of finished
   tweens is nonzero), false while all are still running. */

bool UiCollectionMenuUpdateFrameTween(UiCollectionMenu *self, u8 out)

{
  u8 finished = 0;
  int i;

  for (i = 20; i < 25; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, self->slideDuration, out,
                             ((GfxSprite **)(self->base).data)[i], &self->tweens[i], 1);
  }
  return finished != 0;
}

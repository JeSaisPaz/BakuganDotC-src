// bdc 0x08988ff8 UiCollectionTheaterUpdateArrowTween
#include "bdc.h"

/* Advances the 6 page arrow tweens of `UiCollectionTheater`
   (`UiTweenUpdate` flags 1, 16 frames, scale 1->1); returns true once any of
   them has finished (count of finished tweens != 0). The sprite table
   `base.data` is re-read every iteration, after the previous call. */

bool UiCollectionTheaterUpdateArrowTween(UiCollectionTheater *self, u8 out)
{
  u8 finished = 0;
  int i;

  for (i = 0; i < 6; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out,
                              ((GfxSprite **)self->base.data)[0x19 + i],
                              &self->arrowTweens[i], 1);
  }
  return finished != 0;
}

// bdc 0x08988518 UiCollectionTheaterUpdateBgTween
#include "bdc.h"

/* Advances the background sprite tweens of `UiCollectionTheater`
   (`UiTweenUpdate` mode 1, 16 frames, tweens 0..4 on sprites 0x29..0x2d);
   returns true once at least one of them has finished (sum of finished flags != 0). */

bool UiCollectionTheaterUpdateBgTween(UiCollectionTheater *self, u8 out)
{
  u8 finished = 0;
  int i;

  for (i = 0; i < 5; i++) {
    /* the sprite table pointer is re-read from self->base.data every iteration */
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[0x29 + i],
                              &self->bgTweens[i], 1);
  }
  return finished != 0;
}

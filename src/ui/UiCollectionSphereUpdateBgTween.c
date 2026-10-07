// bdc 0x0897a384 UiCollectionSphereUpdateBgTween
#include "bdc.h"

/* Advances the five background sprite tweens (slots 37..41) of
   `UiCollectionSphere` (`UiTweenUpdate` mode 1, 16 frames);
   returns true once ANY of them has finished (sum of the finished flags != 0). */

bool UiCollectionSphereUpdateBgTween(UiCollectionSphere *self, u8 out)

{
  u8 finished;
  int i;

  finished = 0;
  for (i = 37; i < 42; i++) {
    /* the sprite table pointer is re-read from self->base.data every iteration */
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  }
  return finished != 0;
}

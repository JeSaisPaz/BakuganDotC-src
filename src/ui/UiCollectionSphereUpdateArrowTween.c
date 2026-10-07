// bdc 0x0897b0a8 UiCollectionSphereUpdateArrowTween
#include "bdc.h"

/* Advances the page arrow tweens of `UiCollectionSphere`
   (tweens and sprites 21..26, `UiTweenUpdate` flags 1, 16 frames, fade-out
   when `out`); returns true once any of them has finished. */

bool UiCollectionSphereUpdateArrowTween(UiCollectionSphere *self, u8 out)

{
  u8 finished;
  int i;

  finished = 0;
  for (i = 0x15; i < 0x1b; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out,
                              ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  }
  return finished != 0;
}

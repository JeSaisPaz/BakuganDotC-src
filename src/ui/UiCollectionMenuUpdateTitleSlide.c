// bdc 0x08974a10 UiCollectionMenuUpdateTitleSlide
#include "bdc.h"

/* Advances the title slide tweens 18-19 of `UiCollectionMenu`
   (`UiTweenUpdate` flags 5, duration `slideDuration`); returns true once at least one
   of them has finished. */

bool UiCollectionMenuUpdateTitleSlide(UiCollectionMenu *self, u8 out)
{
  u8 finished = 0;
  int i;

  for (i = 18; i < 20; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, self->slideDuration, out,
                              ((GfxSprite **)self->base.data)[i], &self->tweens[i], 5);
  }
  return finished != 0;
}

// bdc 0x08975eb0 UiCollectionMenuUpdateTitleFade
#include "bdc.h"

/* Advances the two title fade tweens (18 and 19) of `UiCollectionMenu`
   (`UiTweenUpdate` mode 1); returns true once at least one of them has finished. */

bool UiCollectionMenuUpdateTitleFade(UiCollectionMenu *self, u8 out)

{
  u8 finished = 0;
  int i;

  for (i = 18; i < 20; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, self->slideDuration, out,
                             ((GfxSprite **)(self->base).data)[i], &self->tweens[i], 1);
  }
  return finished != 0;
}

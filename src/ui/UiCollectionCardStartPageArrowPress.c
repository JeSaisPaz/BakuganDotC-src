// bdc 0x08984920 UiCollectionCardStartPageArrowPress
#include "bdc.h"

/* Starts the 4-frame press animation (`UiFlashStartRgb`) of the page arrow of direction `+0xbcf`
   of `UiCollectionCard`. */

void UiCollectionCardStartPageArrowPress(UiCollectionCard *self)

{
  UiFlashStartRgb(4.0f, ((GfxSprite **)self->base.data)[self->pageDir * 3 + 16], 1, 0, 0, 1, 1);
  return;
}


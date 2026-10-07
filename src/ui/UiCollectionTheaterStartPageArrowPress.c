// bdc 0x08989c64 UiCollectionTheaterStartPageArrowPress
#include "bdc.h"

/* Starts the 4-frame press animation (`UiFlashStartRgb`) of the page arrow of direction `+0x8e3`
   of `UiCollectionTheater`. */

void UiCollectionTheaterStartPageArrowPress(UiScreen *screen)

{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  GfxSprite **table = (GfxSprite **)screen->data;

  UiFlashStartRgb(4.0f, table[(signed char)self->pageDir * 3 + 0x18], 1, 0, 0, 1, 1);
}

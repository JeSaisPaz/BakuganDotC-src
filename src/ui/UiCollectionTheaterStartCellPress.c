// bdc 0x08989fe4 UiCollectionTheaterStartCellPress
#include "bdc.h"

/* Starts the 4-frame press animation (`UiFlashStart`) of the selected scene cell of
   `UiCollectionTheater`. */

void UiCollectionTheaterStartCellPress(UiScreen *screen)
{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  UiFlashStart(4.0f, ((GfxSprite **)screen->data)[self->cursor], 0, 0);
}

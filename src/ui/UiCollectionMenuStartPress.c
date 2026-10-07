// bdc 0x08975918 UiCollectionMenuStartPress
#include "bdc.h"

/* Starts the 4-frame press animation (`UiFlashStart`) of the selected entry button of
   `UiCollectionMenu`. */

void UiCollectionMenuStartPress(UiCollectionMenu *self)

{
  UiFlashStart(4.0f, ((GfxSprite **)self->base.data)[13 + (&self->selMain)[self->page]], 0, 0);
  return;
}

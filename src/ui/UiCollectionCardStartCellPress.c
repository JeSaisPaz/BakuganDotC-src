// bdc 0x08984c44 UiCollectionCardStartCellPress
#include "bdc.h"

/* Starts the 4-frame press animation (`UiFlashStart`) of the selected card cell of
   `UiCollectionCard`. */

void UiCollectionCardStartCellPress(UiCollectionCard *self)
{
  UiFlashStart(4.0f, ((GfxSprite **)self->base.data)[self->cursor], 0, 0);
}

// bdc 0x0897c6a8 UiCollectionSphereStartCellPress
#include "bdc.h"

/* Starts the 4-frame press animation (`UiFlashStart`) of the selected cell of
   `UiCollectionSphere`. */

void UiCollectionSphereStartCellPress(UiCollectionSphere *self)
{
  UiFlashStart(4.0f, ((GfxSprite **)self->base.data)[self->cursor], 0, 0);
}

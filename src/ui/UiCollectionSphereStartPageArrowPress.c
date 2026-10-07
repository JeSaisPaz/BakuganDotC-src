// bdc 0x0897c21c UiCollectionSphereStartPageArrowPress
#include "bdc.h"

/* Starts the 4-frame press animation (`UiFlashStartRgb`) of the page arrow of side `+0xee3` of
   `UiCollectionSphere`. */

void UiCollectionSphereStartPageArrowPress(UiCollectionSphere *self)

{
  UiFlashStartRgb(4.0f, ((GfxSprite **)self->base.data)[self->pageDir * 3 + 20], 1, 0, 0, 1, 1);
  return;
}


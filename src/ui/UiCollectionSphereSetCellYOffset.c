// bdc 0x0897a028 UiCollectionSphereSetCellYOffset
#include "bdc.h"

/* Sets the camera Y offset of `UiCollectionSphere`: 12 for cell
   5 of page 0 when it is a special page, else 0. */

void UiCollectionSphereSetCellYOffset(UiCollectionSphere *self, u8 page, u8 cell)
{
    self->cameraY = 0.0f;
    if (UiCollectionSphereGetPageKind(self, page) == 0 && page == 0 && cell == 5) {
        self->cameraY = 12.0f;
    }
}

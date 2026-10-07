// bdc 0x0897b748 UiCollectionSphereEnableModelGlow
#include "bdc.h"

/* Resets the cell-model glow record of `UiCollectionSphere` (`+0x1300`,
   0x0c bytes) and enables (1) or disables it. */

void UiCollectionSphereEnableModelGlow(UiCollectionSphere *self, u8 enable)

{
  memset(&self->bobOn,0,0xc);
  self->bobOn = enable;
  return;
}


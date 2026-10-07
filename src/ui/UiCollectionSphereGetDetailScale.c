// bdc 0x0897c774 UiCollectionSphereGetDetailScale
#include "bdc.h"

/* Returns the detail-view scale of model `slot` of `UiCollectionSphere` by
   its model type (record byte `+0x1140 + slot*0x28`): 0.18 for type 1, 0.2 for type 2, else 0.4. */

float UiCollectionSphereGetDetailScale(UiCollectionSphere *self, u8 slot)

{
  u8 type = *(&self->cellType0 + (u32)slot * 0x28);

  if (type < 2) {
    if (type != 0) {
      return 0.18f;
    }
  } else if (type < 3) {
    return 0.2f;
  }
  return 0.4f;
}

// bdc 0x0897b274 UiCollectionSphereGetModelTilt
#include "bdc.h"

/* Returns the initial tilt of cell model `cell` of `UiCollectionSphere`: 0
   for model types 1/2 (record byte `+0x1140 + cell*0x28`), else −0.78 rad. */

float UiCollectionSphereGetModelTilt(UiCollectionSphere *self, u8 cell)

{
  const u8 *type = &self->cellType0 + (u32)cell * 0x28;

  if ((*type != 0) && (*type < 3)) {
    return 0.0f;
  }
  return -0.78f;
}


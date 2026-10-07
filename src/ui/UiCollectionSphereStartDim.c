// bdc 0x0897c91c UiCollectionSphereStartDim
#include "bdc.h"

/* Resets dim record `index` of `UiCollectionSphere` (`+0xef0 +
   index*0x10`) for fading the background dim in (`out` = 0) or out (start 0.7). */

void UiCollectionSphereStartDim(UiCollectionSphere *self, u8 out, u8 index)

{
  UiDimFade *dim = &self->dim[index];

  memset(dim, 0, sizeof(UiDimFade));
  dim->active = 1;
  if (out != 0) {
    dim->value = 0.7f;
    dim->base = 0.7f;
  }
  return;
}

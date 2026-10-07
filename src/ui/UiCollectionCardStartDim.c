// bdc 0x08984d78 UiCollectionCardStartDim
#include "bdc.h"

/* Resets dim record `index` of `UiCollectionCard` (`+0xcb4 + index*0x10`)
   for fading the background dim in (`out` = 0) or out (start 0.7). */

void UiCollectionCardStartDim(UiCollectionCard *self, u8 out, u8 index)

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

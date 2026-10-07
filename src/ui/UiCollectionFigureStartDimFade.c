// bdc 0x0898ee48 UiCollectionFigureStartDimFade
#include "bdc.h"

/* Starts one of the two background-dimming fades of `UiCollectionFigure`
   (record `+0xe88 + layer * 0x10`: {u8 active, float value, float base, float t}); fading out
   starts from 0.7. Advanced by `UiCollectionFigureDimFadeDone`; layer 0 dims behind the detail
   view, layer 1 behind the second detail view. */

void UiCollectionFigureStartDimFade(UiCollectionFigure *self, u8 out, u8 layer)

{
  UiDimFade *dim = &self->dim[layer];

  memset(dim, 0, sizeof(UiDimFade));
  dim->active = 1;
  if (out != 0) {
    dim->value = 0.7f;
    dim->base = 0.7f;
  }
  return;
}

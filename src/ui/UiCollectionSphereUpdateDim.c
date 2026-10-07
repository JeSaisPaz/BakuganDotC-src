// bdc 0x0897c994 UiCollectionSphereUpdateDim
#include "bdc.h"

/* Advances dim record `index` of `UiCollectionSphere` by 1/16 per frame
   (ease-out to 0.7, or ease-in to 0). Returns 1 when finished. */

u8 UiCollectionSphereUpdateDim(UiCollectionSphere *self, u8 out, u8 index)
{
  UiDimFade *dim = &self->dim[index];
  float t = dim->t + 0.0625f;
  float base = dim->base;
  u8 done = 0;

  if (out == 0) {
    dim->t = t;
    dim->value = base + (1.0f - (t - 1.0f) * (t - 1.0f)) * 0.7f;
    if (!(t < 1.0f)) {
      dim->value = 0.7f;
      return 1;
    }
  } else {
    dim->t = t;
    dim->value = base - t * t * 0.7f;
    if (!(t < 1.0f)) {
      done = 1;
      dim->value = 0.0f;
    }
  }
  return done;
}

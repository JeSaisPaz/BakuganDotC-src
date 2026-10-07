// bdc 0x08975d00 UiCollectionMenuUpdateItemBoxZoom
#include "bdc.h"

/* Advances the item-box zoom of `UiCollectionMenu` (step 1/`+0x54c`): eases
   the model alpha in to 1 or out. Returns 1 when finished. */

u8 UiCollectionMenuUpdateItemBoxZoom(UiCollectionMenu *self, u8 out)
{
  u8 done;
  float t;
  float d;

  done = 0;
  if (out == 0) {
    t = self->spinT + 1.0f / self->slideDuration;
    d = t - 1.0f;
    self->spinT = t;
    self->itemBox->ambient[3] = self->spinScale + (1.0f - d * d);
    if (!(self->spinT < 1.0f)) {
      self->itemBox->ambient[3] = 1.0f;
      return 1;
    }
  }
  else {
    t = self->spinT + 1.0f / self->slideDuration;
    self->spinT = t;
    self->itemBox->ambient[3] = self->spinScale - t * t;
    if (!(self->spinT < 1.0f)) {
      done = 1;
    }
  }
  return done;
}

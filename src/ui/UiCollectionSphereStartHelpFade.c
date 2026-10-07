// bdc 0x0897d1a8 UiCollectionSphereStartHelpFade
#include "bdc.h"

/* Resets the description fade of `UiCollectionSphere` (`+0xf20`) with
   alpha 0 (`out` = 0) or 1. */

void UiCollectionSphereStartHelpFade(UiCollectionSphere *self, u8 out)

{
  if (out == '\0') {
    self->descFade = 0.0f;
    self->helpAlpha = 0.0f;
    return;
  }
  self->descFade = 0.0f;
  self->helpAlpha = 1.0f;
  return;
}


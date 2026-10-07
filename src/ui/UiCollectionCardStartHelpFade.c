// bdc 0x08985710 UiCollectionCardStartHelpFade
#include "bdc.h"

/* Resets the description fade of `UiCollectionCard` (`+0xce8`) with alpha 0
   (`out` = 0) or 1. */

void UiCollectionCardStartHelpFade(UiCollectionCard *self, u8 out)

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


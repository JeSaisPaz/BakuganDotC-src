// bdc 0x08921714 UiHologramGalleryWantsAltAction
#include "bdc.h"

/* Returns 1 when SQUARE (pad byte `+5` bit 0x80) is pressed in menu mode 2, else 0. */

int UiHologramGalleryWantsAltAction(UiHologramGallery *self)
{
  if (((s8)(self->base.pad->pressed >> 8) & 0x80) != 0 && self->menuMode == 2) {
    return 1;
  }
  return 0;
}

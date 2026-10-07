// bdc 0x089216cc UiHologramGalleryWantsHelp
#include "bdc.h"

/* Returns 1 when CIRCLE (pad byte `+5` bit 0x20) is pressed on panels 0..2 in menu mode 1, else 0.
    */

int UiHologramGalleryWantsHelp(UiHologramGallery *self)

{
  if (((((((self->base).pad)->pressed & 0x2000) != 0) && (-1 < self->panel)) &&
      (self->panel < '\x03')) && (self->menuMode == '\x01')) {
    return 1;
  }
  return 0;
}


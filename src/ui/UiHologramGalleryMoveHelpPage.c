// bdc 0x08921ee4 UiHologramGalleryMoveHelpPage
#include "bdc.h"

/* Moves the help page `+0x76` of the hologram gallery screen (`UiHologramGalleryCtor`, task 391;
   menu cursor `+0x77`, panel `+0x74`) with UP/DOWN (pad repeat bits 0x10/0x40) within the seen
   pages (`+0x224d`); returns 1 when it moved. */

int UiHologramGalleryMoveHelpPage(UiHologramGallery *self)

{
  PadState *pad;
  
  pad = (self->base).pad;
  if ((pad->repeat & 0x10) == 0) {
    if (((pad->repeat & 0x40) != 0) && (self->helpPage != '\0')) {
      self->helpPage = self->helpPage + -1;
      return 1;
    }
  }
  else if ((int)self->helpPage < (int)(self->helpPages - 1)) {
    self->helpPage = self->helpPage + '\x01';
    return 1;
  }
  return 0;
}


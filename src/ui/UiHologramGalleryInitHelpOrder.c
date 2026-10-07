// bdc 0x0891bef8 UiHologramGalleryInitHelpOrder
#include "bdc.h"

/* Fills the six-byte help page order `+0x224e` of the hologram gallery screen
   (`UiHologramGalleryCtor`, task 391; menu cursor `+0x77`, panel `+0x74`) with {1, 0, 4, 3, 2,
   5}. */

void UiHologramGalleryInitHelpOrder(UiHologramGallery *self)
{
  u8 order[16];
  int i;

  order[0] = 1;
  order[1] = 0;
  order[2] = 4;
  order[3] = 3;
  order[4] = 2;
  order[5] = 5;
  for (i = 0; i < 6; i++) {
    self->helpOrder[i] = order[i];
  }
}

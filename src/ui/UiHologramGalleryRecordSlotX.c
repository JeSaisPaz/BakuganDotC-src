// bdc 0x0891c9d4 UiHologramGalleryRecordSlotX
#include "bdc.h"

/* Stores the x positions of the six slot sprites (`+0x218..`) into `+0x2264..` of the hologram
   gallery screen (`UiHologramGalleryCtor`, task 391; menu cursor `+0x77`, panel `+0x74`). */

void UiHologramGalleryRecordSlotX(UiHologramGallery *self)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  int i;

  for (i = 0; i < 6; i++) {
    self->slotX[i] = sprites[0x218 / 4 + i]->posX;
  }
}

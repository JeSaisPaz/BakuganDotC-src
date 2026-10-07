// bdc 0x0891ca08 UiHologramGalleryInitScrollArrows
#include "bdc.h"

/* Records the y positions of the four scroll-arrow sprites 0x51..0x54 of the hologram gallery
   screen (`UiHologramGalleryCtor`, task 391) into `arrowY` and flips every second one
   vertically (`GfxSpriteFlipV`). */

void UiHologramGalleryInitScrollArrows(UiHologramGallery *self)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  int i;

  for (i = 0; i < 4; i++) {
    self->arrowY[i] = sprites[0x51 + i]->posY;
    if (i & 1) {
      GfxSpriteFlipV(sprites[0x51 + i]);
    }
  }
}

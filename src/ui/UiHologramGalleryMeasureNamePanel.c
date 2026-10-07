// bdc 0x0891c5f8 UiHologramGalleryMeasureNamePanel
#include "bdc.h"

/* Records the offsets of the name label sprites (`+0xdc`, `+0xd8`) from the name panel sprite
   `+0xd4` of the hologram gallery screen (`UiHologramGalleryCtor`, task 391; menu cursor `+0x77`,
   panel `+0x74`) into `+0x217c..+0x2188`. */

void UiHologramGalleryMeasureNamePanel(UiHologramGallery *self)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  self->nameLabelADx = sprites[0xd4 / 4]->posX - sprites[0xdc / 4]->posX;
  self->nameLabelADy = (sprites[0xd4 / 4]->posY - sprites[0xdc / 4]->posY) - 2.0f;
  self->nameLabelBDx = sprites[0xd4 / 4]->posX - sprites[0xd8 / 4]->posX;
  self->nameLabelBDy = (sprites[0xd4 / 4]->posY - sprites[0xd8 / 4]->posY) - 1.0f;
}

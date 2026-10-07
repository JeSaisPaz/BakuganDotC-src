// bdc 0x08920c48 UiHologramGalleryStartListPulse
#include "bdc.h"

/* Resets the list pulse records `+0x21dc` of the hologram gallery screen
   (`UiHologramGalleryCtor`, task 391; menu cursor `+0x77`, panel `+0x74`), clears the list
   sprites' colours and arms the pulse (`on`) for the entry matching the selected slot's hologram.
    */

void UiHologramGalleryStartListPulse(UiHologramGallery *self, u8 on)
{
  int i;
  float *pulse;
  GfxSprite **sprites = (GfxSprite **)self->base.data + 0xaa;

  memset(self->listPulse, 0, 0x30);
  for (i = 0; i < self->slotCount; i++) {
    GfxSprite *spr = sprites[i];
    spr->addColor[0] = 0.0f;
    spr->addColor[1] = 0.0f;
    spr->addColor[2] = 0.0f;
    spr->addColor[3] = 1.0f;
    if (self->listMode[0] == 0 && i == self->slot) {
      self->listPulse[i * 12] = on;
      if (self->listPulse[i * 12] == 1) {
        pulse = (float *)&self->listPulse[i * 12 + 8];
        *pulse = 1.0f;
      }
    }
  }
}

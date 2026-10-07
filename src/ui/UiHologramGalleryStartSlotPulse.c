// bdc 0x08920f2c UiHologramGalleryStartSlotPulse
#include "bdc.h"

/* Resets the slot pulse records `+0x21b8` of the hologram gallery screen
   (`UiHologramGalleryCtor`, task 391; menu cursor `+0x77`, panel `+0x74`), clears the slot
   sprites' colours and arms the pulse (`on`) for every existing slot. */

void UiHologramGalleryStartSlotPulse(UiHologramGallery *self, u8 on)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  int i;

  memset(self->slotPulse, 0, 0x24);
  for (i = 0; i < 3; i++) {
    if (self->slotIds[i] != 0xff) {
      GfxSprite *sprite;
      self->slotPulse[i * 12] = on;
      sprite = sprites[0x9f + i];
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 1.0f;
    }
  }
}

// bdc 0x0891e778 UiHologramGalleryPulseSlots
#include "bdc.h"

/* Animates the colour pulse of the existing slot sprites (sprites 159..161 of the data table) of
   the hologram gallery screen (`UiHologramGalleryCtor`, task 391; menu cursor `menuCursor`, panel
   `panel`) whose record `slotPulse` (0xc bytes each) is on and whose slot id is not 0xff: grey level
   `0.3 * (1 - cos(pi t))`, `t` += 0.025. */

void UiHologramGalleryPulseSlots(UiHologramGallery *self)
{
  UiHologramPulse *pulse = (UiHologramPulse *)self->slotPulse;
  float c;
  GfxSprite *sprite;
  float level;
  int i;

  for (i = 0; i < 3; i++) {
    if (pulse[i].on != 0 && self->slotIds[i] != 0xff) {
      pulse[i].t = pulse[i].t + 0.025f;
      c = __builtin_cosf(pulse[i].t * 3.1415927f);
      level = (1.0f - c) * 0.5f * 0.6f;
      pulse[i].level = level;
      sprite = ((UiHologramGalleryData *)self->base.data)->sprites[159 + i];
      sprite->addColor[0] = level;
      sprite->addColor[1] = level;
      sprite->addColor[2] = level;
      sprite->addColor[3] = 1.0f;
    }
  }
}

// bdc 0x0891e84c UiHologramGalleryPulseList
#include "bdc.h"

/* Animates the colour pulse of the hologram list entries (sprites 170.. of the data table, records
   `listPulse`) of the hologram gallery screen (`UiHologramGalleryCtor`, task 391) like
   `UiHologramGalleryPulseSlots` (red/green only). */

void UiHologramGalleryPulseList(UiHologramGallery *self)
{
  UiHologramPulse *pulse = (UiHologramPulse *)self->listPulse;
  float c;
  GfxSprite *sprite;
  float level;
  int i;

  for (i = 0; i < (int)self->slotCount; i++) {
    if (pulse[i].on != 0) {
      pulse[i].t = pulse[i].t + 0.025f;
      c = __builtin_cosf(pulse[i].t * 3.1415927f);
      level = (1.0f - c) * 0.5f * 0.6f;
      pulse[i].level = level;
      sprite = ((UiHologramGalleryData *)self->base.data)->sprites[170 + i];
      sprite->addColor[0] = level;
      sprite->addColor[1] = level;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 1.0f;
    }
  }
}

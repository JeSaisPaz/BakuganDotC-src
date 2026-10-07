// bdc 0x08921e44 UiHologramGalleryStartHelpPages
#include "bdc.h"

/* Arms the help page viewer of the hologram gallery screen (`UiHologramGalleryCtor`, task 391;
   menu cursor `+0x77`, panel `+0x74`) (record `+0x227c = on`, page `+0x227e` from `+0x76`) and
   resets the four scroll arrows 0x51..0x54 (hidden, alpha 1). */

void UiHologramGalleryStartHelpPages(UiHologramGallery *self, char on)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  int i = 0x51;

  memset(&self->pageAnimOn, 0, 8);
  self->pageAnimOn = on;
  do {
    sprites[i]->flags &= ~1u;
    i++;
    sprites[i - 1]->alpha = 1.0f;
  } while (i < 0x55);
  if (on != '\0') {
    self->pageAnimPage = self->helpPage;
  }
}

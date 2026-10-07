// bdc 0x08921f48 UiHologramGalleryResetConfirm
#include "bdc.h"

/* Clears the confirmation record `+0x2294` (12 bytes) of the hologram gallery screen
   (`UiHologramGalleryCtor`, task 391; menu cursor `+0x77`, panel `+0x74`). */

void UiHologramGalleryResetConfirm(UiHologramGallery *self)

{
  memset(self->confirm,0,0xc);
  return;
}


// bdc 0x0891bc1c UiHologramGalleryInitMenuLocks
#include "bdc.h"

/* Computes the disabled-item mask `+0x218c` of the hologram gallery screen
   (`UiHologramGalleryCtor`, task 391; menu cursor `+0x77`, panel `+0x74`): bit 1 when no hologram
   slots exist (`+0x2137 == 0`), bits 0 and 2 while `UiHologramGalleryIsTutorialLocked`. */

void UiHologramGalleryInitMenuLocks(UiHologramGallery *self)

{
  int locked;
  
  self->disabledItems = '\0';
  if (self->slotCount == '\0') {
    self->disabledItems = self->disabledItems | 2;
  }
  else {
    locked = UiHologramGalleryIsTutorialLocked();
    if (locked == 1) {
      self->disabledItems = self->disabledItems | 5;
    }
  }
  return;
}


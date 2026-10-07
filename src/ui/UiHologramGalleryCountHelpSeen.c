// bdc 0x0891be34 UiHologramGalleryCountHelpSeen
#include "bdc.h"

/* Counts the first-visit help bits set in profile byte `+0x88` (bits 0..2) into `+0x224d` of the
   hologram gallery screen (`UiHologramGalleryCtor`, task 391; menu cursor `+0x77`, panel
   `+0x74`). */

void UiHologramGalleryCountHelpSeen(UiHologramGallery *self)
{
  u8 count = 0;
  int i;

  for (i = 0; i < 3; i++) {
    SaveProfile *profile = SaveGetProfile();
    if ((profile->data->helpSeen[(u8)i >> 3] & (1 << ((u8)i & 7))) != 0) {
      count++;
    }
  }
  self->helpPages = count;
}

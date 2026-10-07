// bdc 0x089223f8 UiHologramGalleryStartBonus
#include "bdc.h"

/* Arms the point-bonus counter of the hologram gallery screen (`UiHologramGalleryCtor`, task 391;
   menu cursor `+0x77`, panel `+0x74`): clears the record `+0x2254` and sets the amount `+0x2258 =
   300`. */

void UiHologramGalleryStartBonus(UiHologramGallery *self)
{
  memset(self->bonusRecord, 0, 0x10);
  self->bonusAmount = 300;
}

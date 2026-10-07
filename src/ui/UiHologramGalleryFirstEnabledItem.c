// bdc 0x0891bc78 UiHologramGalleryFirstEnabledItem
#include "bdc.h"

/* Puts the menu cursor `+0x77` of the hologram gallery screen (`UiHologramGalleryCtor`, task 391;
   menu cursor `+0x77`, panel `+0x74`) on the first of the 3 items not disabled in `+0x218c`. */

void UiHologramGalleryFirstEnabledItem(UiHologramGallery *self)
{
  int i = 0;
  int cur;

  do {
    cur = i;
    if ((self->disabledItems & (1 << cur)) == 0) break;
    i = cur + 1;
  } while (i < 3);
  self->menuCursor = (s8)cur;
}

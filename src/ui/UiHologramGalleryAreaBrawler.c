// bdc 0x0891fa70 UiHologramGalleryAreaBrawler
#include "bdc.h"

/* Returns the brawler portrait index for area id `area`: 5 for 0x25, else `area / 4` capped at 6.
    */

u32 UiHologramGalleryAreaBrawler(UiHologramGallery *self, u32 area)

{
  u32 brawler;

  if ((area & 0xff) == 0x25) {
    return 5;
  }
  brawler = (int)(area & 0xff) >> 2;
  if (6 < brawler) {
    brawler = 6;
  }
  return brawler;
}

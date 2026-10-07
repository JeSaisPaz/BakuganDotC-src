// bdc 0x0891f940 UiHologramGalleryInfoPanelSprite
#include "bdc.h"

/* Returns entry `i` of the info-panel sprite list `g_hologramGalleryInfoPanelSprites` (terminated by 0xff). */

u32 UiHologramGalleryInfoPanelSprite(UiHologramGallery *self, u32 i)

{
  u32 list[30];

  memcpy(list, g_hologramGalleryInfoPanelSprites, 0x78);
  return list[i & 0xff];
}

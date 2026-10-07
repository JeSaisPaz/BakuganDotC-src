// bdc 0x08921748 UiHologramGalleryHelpPanelSprite
#include "bdc.h"

/* Returns entry `i` of the help-panel sprite list `g_hologramHelpPanelSprites` (terminated by 0xff). */

u8 UiHologramGalleryHelpPanelSprite(UiHologramGallery *self, u32 i)
{
  u8 list[16];

  memcpy(list, g_hologramHelpPanelSprites, 0xe);
  return list[i & 0xff];
}

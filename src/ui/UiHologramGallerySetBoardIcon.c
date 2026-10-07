// bdc 0x0891fb38 UiHologramGallerySetBoardIcon
#include "bdc.h"

/* Sets a sprite's texture to the board icon for `kind`: 1 → `fix_dc_05`, 2 → `fix_dc_02`, 3 →
   `fix_dc_07`, 4 → `fix_dc_04`, 5 → `fix_dc_06`, else `fix_dc_01`. `self` is unused. */

void UiHologramGallerySetBoardIcon(UiHologramGallery *self, GfxSprite *sprite, u8 kind)
{
  char name[64];

  switch (kind) {
  case 1:
    sprintf(name, "fix_dc_05");
    break;
  case 2:
    sprintf(name, "fix_dc_02");
    break;
  case 3:
    sprintf(name, "fix_dc_07");
    break;
  case 4:
    sprintf(name, "fix_dc_04");
    break;
  case 5:
    sprintf(name, "fix_dc_06");
    break;
  default:
    sprintf(name, "fix_dc_01");
    break;
  }
  sprite->texture = GfxFindTexture(name);
}

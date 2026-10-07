// bdc 0x0891d08c UiHologramGallerySetItemFrameLit
#include "bdc.h"

/* Sets a list frame sprite's texture: `fix_waku_01_2` when `lit`, else `fix_waku_01_1`. */

void UiHologramGallerySetItemFrameLit(UiHologramGallery *self, GfxSprite *sprite, bool lit)
{
  char name[64];

  if (lit) {
    sprintf(name, "fix_waku_01_2");
  } else {
    sprintf(name, "fix_waku_01_1");
  }
  sprite->texture = GfxFindTexture(name);
}

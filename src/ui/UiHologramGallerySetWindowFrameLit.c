// bdc 0x0891d030 UiHologramGallerySetWindowFrameLit
#include "bdc.h"

/* Sets a window frame sprite's texture: `fix_waku_01_a` when `lit`, else `fix_waku_01_b`. */

void UiHologramGallerySetWindowFrameLit(UiHologramGallery *self, GfxSprite *sprite, bool lit)
{
  char name[64];

  if (lit) {
    sprintf(name, "fix_waku_01_a");
  } else {
    sprintf(name, "fix_waku_01_b");
  }
  sprite->texture = GfxFindTexture(name);
}

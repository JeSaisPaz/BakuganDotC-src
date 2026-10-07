// bdc 0x0891d0e8 UiHologramGallerySetSlotFrameLit
#include "bdc.h"

/* Sets a slot frame sprite's texture: `fix_waku_02_2` when `lit`, else `fix_waku_02_1`. */

void UiHologramGallerySetSlotFrameLit(UiHologramGallery *self, GfxSprite *sprite, bool lit)

{
  char name[64];
  
  if (lit) {
    sprintf(name,"fix_waku_02_2");
  }
  else {
    sprintf(name,"fix_waku_02_1");
  }
  sprite->texture = GfxFindTexture(name);
  return;
}


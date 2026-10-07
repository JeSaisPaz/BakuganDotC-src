// bdc 0x08918ca4 UiAdvSelectSetBakuganPicture
#include "bdc.h"

/* Sets the texture of a sprite to the Bakugan picture `adv_baku_%02d` (`lit`) or its shadow
   `adv_baku_shadow%02d`. */

void UiAdvSelectSetBakuganPicture(UiAdvSelect *self, GfxSprite *sprite, u32 id, bool lit)

{
  char name[64];
  
  if (lit) {
    sprintf(name,"adv_baku_%02d",id & 0xff);
  }
  else {
    sprintf(name,"adv_baku_shadow%02d",id & 0xff);
  }
  sprite->texture = GfxFindTexture(name);
}


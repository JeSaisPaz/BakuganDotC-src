// bdc 0x08919854 UiAdvSelectSetBakuganName
#include "bdc.h"

/* Sets a sprite's texture to the Bakugan name image `f_cha_name_baku_%02d` (id 0 →
   `f_cha_name_baku_21`). */

void UiAdvSelectSetBakuganName(UiAdvSelect *self, GfxSprite *sprite, u32 id)

{
  char name[64];
  
  if ((id & 0xff) == 0) {
    sprintf(name,"f_cha_name_baku_21");
  }
  else {
    sprintf(name,"f_cha_name_baku_%02d",id & 0xff);
  }
  sprite->texture = GfxFindTexture(name);
}


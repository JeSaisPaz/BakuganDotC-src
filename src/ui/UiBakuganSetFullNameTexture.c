// bdc 0x0892c860 UiBakuganSetFullNameTexture
#include "bdc.h"

/* Sets a sprite's texture to the Bakugan name image `f_cha_name_baku_%02d`. */

void UiBakuganSetFullNameTexture(GfxSprite *sprite, u32 id)

{
  char name[64];
  
  sprintf(name,"f_cha_name_baku_%02d",id & 0xff);
  sprite->texture = GfxFindTexture(name);
  return;
}


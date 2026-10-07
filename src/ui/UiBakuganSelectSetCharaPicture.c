// bdc 0x0892e5d0 UiBakuganSelectSetCharaPicture
#include "bdc.h"

/* Sets a sprite's texture to `f_cus_chara_%02d`. */

void UiBakuganSelectSetCharaPicture(UiBakuganSelect *self, GfxSprite *sprite, u32 index)

{
  char name[64];
  
  sprintf(name,"f_cus_chara_%02d",index & 0xff);
  sprite->texture = GfxFindTexture(name);
  return;
}


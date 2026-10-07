// bdc 0x08919914 UiAdvSelectSetCharaPicture
#include "bdc.h"

/* Sets a sprite's texture to the character portrait `adv_chara_%02d`. */

void UiAdvSelectSetCharaPicture(UiAdvSelect *self, GfxSprite *sprite, u32 id)

{
  void *tex;
  char name [64];
  
  sprintf(name,"adv_chara_%02d",id & 0xff);
  tex = GfxFindTexture(name);
  sprite->texture = tex;
  return;
}


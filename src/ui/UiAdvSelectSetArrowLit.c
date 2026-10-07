// bdc 0x08918ff4 UiAdvSelectSetArrowLit
#include "bdc.h"

/* Sets an arrow sprite's texture: `adv_yaji_01` when `lit`, else `adv_yaji_02`. */

void UiAdvSelectSetArrowLit(UiAdvSelect *self, GfxSprite *sprite, bool lit)

{
  void *texture;
  char name [64];
  
  if (lit) {
    sprintf(name,"adv_yaji_01");
  }
  else {
    sprintf(name,"adv_yaji_02");
  }
  texture = GfxFindTexture(name);
  sprite->texture = texture;
  return;
}


// bdc 0x0892e7f8 UiBakuganSelectSetArrowLit
#include "bdc.h"

/* Sets an arrow sprite's texture: `f_cus_yaji_01` when `lit`, else `f_cus_yaji_02`. */

void UiBakuganSelectSetArrowLit(UiBakuganSelect *self, GfxSprite *sprite, bool lit)

{
  void *texture;
  char name[64];

  if (lit) {
    sprintf(name, "f_cus_yaji_%02d", 1);
  }
  else {
    sprintf(name, "f_cus_yaji_%02d", 2);
  }
  texture = GfxFindTexture(name);
  sprite->texture = texture;
  return;
}

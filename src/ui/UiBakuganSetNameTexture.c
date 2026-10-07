// bdc 0x0892bd48 UiBakuganSetNameTexture
#include "bdc.h"

/* Sets a sprite's texture to `cha_name_baku_%02d` for Bakugan `id` (`id - 1`, or 0 for id 0). */

void UiBakuganSetNameTexture(GfxSprite *sprite, u32 id)

{
  void *texture;
  char name[64];

  if ((id & 0xff) == 0) {
    sprintf(name, "cha_name_baku_%02d", 0);
  }
  else {
    sprintf(name, "cha_name_baku_%02d", (id & 0xff) - 1);
  }
  texture = GfxFindTexture(name);
  sprite->texture = texture;
  return;
}

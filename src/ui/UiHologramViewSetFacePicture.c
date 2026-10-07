// bdc 0x08929e64 UiHologramViewSetFacePicture
#include "bdc.h"

/* Sets a sprite's texture to the narrator face `tips_kao%01d`. */

void UiHologramViewSetFacePicture(UiHologramView *self, GfxSprite *sprite, u32 index)
{
  char name[64];

  sprintf(name, "tips_kao%01d", index & 0xff);
  sprite->texture = GfxFindTexture(name);
}

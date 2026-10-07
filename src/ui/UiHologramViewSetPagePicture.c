// bdc 0x089297fc UiHologramViewSetPagePicture
#include "bdc.h"

/* Sets a sprite's texture to the page picture `fix_tyu_%02d`. */

void UiHologramViewSetPagePicture(UiHologramView *self, GfxSprite *sprite, u32 index)
{
  char name[64];

  sprintf(name, "fix_tyu_%02d", index & 0xff);
  sprite->texture = GfxFindTexture(name);
}

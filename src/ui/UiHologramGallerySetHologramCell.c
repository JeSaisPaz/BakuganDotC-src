// bdc 0x0891fc48 UiHologramGallerySetHologramCell
#include "bdc.h"

/* Sets a sprite's cell to (`base + index / 3`, `index % 3`). */

void UiHologramGallerySetHologramCell(UiHologramGallery *self, GfxSprite *sprite, u32 index, u32 base)

{
  GfxSpriteSetCell(sprite,(float)((base & 0xff) + (index & 0xff) / 3),(float)((index & 0xff) % 3));
  return;
}


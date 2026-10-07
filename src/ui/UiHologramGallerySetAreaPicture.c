// bdc 0x0891faf4 UiHologramGallerySetAreaPicture
#include "bdc.h"

/* Sets a sprite's texture to the field area picture `fix_f%01d_area_%02d` (`field`, `area`). */

void UiHologramGallerySetAreaPicture(UiHologramGallery *self, GfxSprite *sprite, u32 field, u32 area)

{
  char name[64];

  sprintf(name,"fix_f%01d_area_%02d",field & 0xff,area & 0xff);
  sprite->texture = GfxFindTexture(name);
  return;
}

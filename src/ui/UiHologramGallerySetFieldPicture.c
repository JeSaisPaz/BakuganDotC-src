// bdc 0x0891fc9c UiHologramGallerySetFieldPicture
#include "bdc.h"

/* Sets a sprite's texture to the field picture `fix_f%01d_%02d` (`field`, `index + 1`). */

void UiHologramGallerySetFieldPicture(UiHologramGallery *self, GfxSprite *sprite, u32 field, u32 index)

{
  void *tex;
  char name [64];
  
  sprintf(name,"fix_f%01d_%02d",field & 0xff,(index & 0xff) + 1);
  tex = GfxFindTexture(name);
  sprite->texture = tex;
  return;
}


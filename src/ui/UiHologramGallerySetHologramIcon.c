// bdc 0x0891fce4 UiHologramGallerySetHologramIcon
#include "bdc.h"

/* Sets a sprite's texture to the hologram icon `fix_ht_a_%02d_%02d` (`attr + 1`, `variant + 1`). */

void UiHologramGallerySetHologramIcon(UiHologramGallery *self, GfxSprite *sprite, u32 variant, u32 attr)

{
  void *tex;
  char name [64];
  
  sprintf(name,"fix_ht_a_%02d_%02d",(attr & 0xff) + 1,(variant & 0xff) + 1);
  tex = GfxFindTexture(name);
  sprite->texture = tex;
  return;
}


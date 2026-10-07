// bdc 0x0891fab4 UiHologramGallerySetBrawlerPicture
#include "bdc.h"

/* Sets a sprite's texture to the brawler portrait `fix_bri_chara_%02d`. */

void UiHologramGallerySetBrawlerPicture(UiHologramGallery *self, GfxSprite *sprite, u32 index)
{
  char name[64];

  sprintf(name, "fix_bri_chara_%02d", index & 0xff);
  sprite->texture = GfxFindTexture(name);
}

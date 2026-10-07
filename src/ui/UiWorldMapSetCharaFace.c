// bdc 0x0899cff4 UiWorldMapSetCharaFace
#include "bdc.h"

/* Sets a sprite of `UiWorldMap`'s stage list to character portrait `id`
   (`"f_cus_chara_%02d"`). */

void UiWorldMapSetCharaFace(UiScreen *screen, GfxSprite *sprite, u8 id)
{
  char name[64];

  sprintf(name, "f_cus_chara_%02d", id);
  sprite->texture = GfxFindTexture(name);
}

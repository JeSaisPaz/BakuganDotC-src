// bdc 0x089890c0 UiCollectionTheaterSetThumbnail
#include "bdc.h"

/* Sets a scene thumbnail sprite of `UiCollectionTheater` to
   `"cinema_%02d"` (scene + 1), or to a grey `"cinema_01"` for a locked slot (0xff). */

void UiCollectionTheaterSetThumbnail(UiScreen *screen, GfxSprite *sprite, u8 scene)

{
  char name[64];
  
  if (scene == 0xff) {
    sprintf(name,"cinema_01");
    sprite->alpha = 0.0f;
    sprite->tint[0] = 0.5f;
    sprite->tint[1] = 0.5f;
    sprite->tint[2] = 0.5f;
  }
  else {
    sprintf(name,"cinema_%02d",scene + 1);
    sprite->alpha = 0.0f;
    sprite->tint[0] = 1.0f;
    sprite->tint[1] = 1.0f;
    sprite->tint[2] = 1.0f;
  }
  sprite->texture = GfxFindTexture(name);
  return;
}


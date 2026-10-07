// bdc 0x0899c514 UiWorldMapSetStagePlate
#include "bdc.h"

/* Sets a stage-list plate sprite of `UiWorldMap` to `"area_ita"` when `alt` is
   set, else `"area_ita01"`. */

void UiWorldMapSetStagePlate(UiScreen *screen, GfxSprite *sprite, bool alt)

{
  char name [64];
  
  if (alt) {
    sprintf(name,"area_ita");
  }
  else {
    sprintf(name,"area_ita01");
  }
  sprite->texture = GfxFindTexture(name);
}


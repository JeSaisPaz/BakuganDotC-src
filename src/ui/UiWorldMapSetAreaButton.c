// bdc 0x08998afc UiWorldMapSetAreaButton
#include "bdc.h"

/* Sets an area button sprite of `UiWorldMap` to `"sele_ita_on_01"` when
   `selected`, else `"sele_ita_off_01"`. */

void UiWorldMapSetAreaButton(UiScreen *screen, GfxSprite *sprite, bool selected)

{
  char name [64];
  
  if (selected) {
    sprintf(name,"sele_ita_on_01");
  }
  else {
    sprintf(name,"sele_ita_off_01");
  }
  sprite->texture = GfxFindTexture(name);
}


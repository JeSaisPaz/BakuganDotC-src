// bdc 0x089885e0 UiCollectionTheaterSetCellFrame
#include "bdc.h"

/* Sets a scene cell frame texture of `UiCollectionTheater`: `"waku_4_a"`
   when selected, `"waku_4_b"` otherwise. */

void UiCollectionTheaterSetCellFrame(UiScreen *screen, GfxSprite *sprite, u8 selected)

{
  char name[64];
  
  if (selected == '\0') {
    sprintf(name,"waku_4_b");
  }
  else {
    sprintf(name,"waku_4_a");
  }
  sprite->texture = GfxFindTexture(name);
  return;
}


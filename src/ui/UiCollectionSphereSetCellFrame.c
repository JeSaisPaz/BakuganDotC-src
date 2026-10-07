// bdc 0x0897a44c UiCollectionSphereSetCellFrame
#include "bdc.h"

/* Sets a cell frame texture of `UiCollectionSphere`: `"waku_4_a"` when
   selected, `"waku_4_b"` otherwise. */

void UiCollectionSphereSetCellFrame(UiCollectionSphere *self, GfxSprite *sprite, u8 selected)

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


// bdc 0x08983194 UiCollectionCardSetCellFrame
#include "bdc.h"

/* Sets a card cell frame texture of `UiCollectionCard`: `"waku_4_a"` when
   selected, `"waku_4_b"` otherwise. */

void UiCollectionCardSetCellFrame(UiCollectionCard *self, GfxSprite *sprite, u8 selected)

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


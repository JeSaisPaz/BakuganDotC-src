// bdc 0x08974ac8 UiCollectionMenuSetButtonFrame
#include "bdc.h"

/* Sets an entry button frame texture of `UiCollectionMenu`: `"waku_1_a"`
   when selected, `"waku_1_b"` otherwise. */

void UiCollectionMenuSetButtonFrame(UiCollectionMenu *self, GfxSprite *sprite, u8 selected)

{
  char name[64];
  
  if (selected == '\0') {
    sprintf(name,"waku_1_b");
  }
  else {
    sprintf(name,"waku_1_a");
  }
  sprite->texture = GfxFindTexture(name);
  return;
}


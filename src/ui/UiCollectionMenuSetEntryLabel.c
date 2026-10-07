// bdc 0x08975a64 UiCollectionMenuSetEntryLabel
#include "bdc.h"

/* Sets the label cell of a main-entry sprite of `UiCollectionMenu` from the
   entry index (0→0, 1→1, 2→3, 3→4, 4→2). */

void UiCollectionMenuSetEntryLabel(UiCollectionMenu *self, GfxSprite *sprite, u8 entry)

{
  int row = 0;
  if (entry < 5) {
    if (entry == 1) {
      row = 1;
    }
    else if (entry == 2) {
      row = 3;
    }
    else if (entry == 3) {
      row = 4;
    }
    else if (entry == 4) {
      row = 2;
    }
    else {
      row = 0;
    }
  }
  GfxSpriteSetCell(sprite,0.0f,(float)row);
  return;
}

// bdc 0x0898c754 UiCollectionFigureSetCellFrame
#include "bdc.h"

/* Sets the frame texture of a grid cell sprite of `UiCollectionFigure`:
   `"waku_4_a"` when `selected`, else `"waku_4_b"` (`GfxFindTexture` → `sprite->texture`). */

void UiCollectionFigureSetCellFrame(UiCollectionFigure *self, GfxSprite *sprite, bool selected)

{
  char name[64];
  
  if (selected) {
    sprintf(name,"waku_4_a");
  }
  else {
    sprintf(name,"waku_4_b");
  }
  sprite->texture = GfxFindTexture(name);
  return;
}


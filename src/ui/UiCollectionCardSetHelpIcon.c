// bdc 0x08985fc4 UiCollectionCardSetHelpIcon
#include "bdc.h"

/* Sets the cell of the button guide sprite 0x22 of `UiCollectionCard`: 5 in
   the detail view (`grid` = 0), 0 back on the grid. */

void UiCollectionCardSetHelpIcon(UiCollectionCard *self, u8 grid)

{
  GfxSprite *sprite;
  
  sprite = ((GfxSprite **)self->base.data)[34];
  if (grid == '\0') {
    GfxSpriteSetCell(sprite,0.0,5.0);
    return;
  }
  GfxSpriteSetCell(sprite,0.0,0.0);
  return;
}


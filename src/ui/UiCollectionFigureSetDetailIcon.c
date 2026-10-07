// bdc 0x0898fb2c UiCollectionFigureSetDetailIcon
#include "bdc.h"

/* Sets the cell of sprite 0x26 (data `+0x98`) of `UiCollectionFigure`: row
   5 on entering the detail view (`closing` = 0), row 0 when it is closed (`GfxSpriteSetCell`). */

void UiCollectionFigureSetDetailIcon(UiCollectionFigure *self, u8 closing)

{
  GfxSprite *sprite;
  
  sprite = ((GfxSprite **)self->base.data)[38];
  if (closing == '\0') {
    GfxSpriteSetCell(sprite,0.0,5.0);
    return;
  }
  GfxSpriteSetCell(sprite,0.0,0.0);
  return;
}


// bdc 0x0897c4f4 UiCollectionSpherePlaceCellSprite
#include "bdc.h"

/* Moves sprite `sprite` of `UiCollectionSphere` to the saved position of
   cell `cell` (`+0xca8 + cell*8`). */

void UiCollectionSpherePlaceCellSprite(UiCollectionSphere *self, u8 sprite, u8 cell)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  sprites[sprite]->posX = self->spritePos[cell][0];
  sprites[sprite]->posY = self->spritePos[cell][1];
  return;
}


// bdc 0x0895be44 UiEquipLayoutGrid
#include "bdc.h"

/* Positions the 20-cell Bakugan grid of the loadout screen (task 302, `UiEquipCtor`) relative to
   the grid base sprite spriteIdx[0]: for each cell i the sprites spriteIdx[1]+i, spriteIdx[2]+i,
   spriteIdx[3]+i and spriteIdx[0x4b]+i are placed at base position + faceOffset[i] * base scale,
   then the cursor sprites spriteIdx[4] and spriteIdx[5] at base position + cursorOffset * base
   scale. The sprite table and the base sprite are re-read after every store. */

#define UIEQ_SPRITE(self, n) (((GfxSprite **)(self)->base.data)[(n)])

void UiEquipLayoutGrid(UiEquip *self)
{
  GfxSprite *base;
  int i;

  for (i = 0; i < 20; i++) {
    base = UIEQ_SPRITE(self, self->spriteIdx[0]);
    UIEQ_SPRITE(self, self->spriteIdx[1] + i)->posX = base->posX + self->faceOffset[i][0] * base->scaleX;
    base = UIEQ_SPRITE(self, self->spriteIdx[0]);
    UIEQ_SPRITE(self, self->spriteIdx[1] + i)->posY = base->posY + self->faceOffset[i][1] * base->scaleY;
    base = UIEQ_SPRITE(self, self->spriteIdx[0]);
    UIEQ_SPRITE(self, self->spriteIdx[2] + i)->posX = base->posX + self->faceOffset[i][0] * base->scaleX;
    base = UIEQ_SPRITE(self, self->spriteIdx[0]);
    UIEQ_SPRITE(self, self->spriteIdx[2] + i)->posY = base->posY + self->faceOffset[i][1] * base->scaleY;
    base = UIEQ_SPRITE(self, self->spriteIdx[0]);
    UIEQ_SPRITE(self, self->spriteIdx[3] + i)->posX = base->posX + self->faceOffset[i][0] * base->scaleX;
    base = UIEQ_SPRITE(self, self->spriteIdx[0]);
    UIEQ_SPRITE(self, self->spriteIdx[3] + i)->posY = base->posY + self->faceOffset[i][1] * base->scaleY;
    base = UIEQ_SPRITE(self, self->spriteIdx[0]);
    UIEQ_SPRITE(self, self->spriteIdx[0x4b] + i)->posX = base->posX + self->faceOffset[i][0] * base->scaleX;
    base = UIEQ_SPRITE(self, self->spriteIdx[0]);
    UIEQ_SPRITE(self, self->spriteIdx[0x4b] + i)->posY = base->posY + self->faceOffset[i][1] * base->scaleY;
  }

  base = UIEQ_SPRITE(self, self->spriteIdx[0]);
  UIEQ_SPRITE(self, self->spriteIdx[4])->posX = base->posX + self->cursorOffset[0] * base->scaleX;
  base = UIEQ_SPRITE(self, self->spriteIdx[0]);
  UIEQ_SPRITE(self, self->spriteIdx[4])->posY = base->posY + self->cursorOffset[1] * base->scaleY;
  base = UIEQ_SPRITE(self, self->spriteIdx[0]);
  UIEQ_SPRITE(self, self->spriteIdx[5])->posX = base->posX + self->cursorOffset[0] * base->scaleX;
  base = UIEQ_SPRITE(self, self->spriteIdx[0]);
  UIEQ_SPRITE(self, self->spriteIdx[5])->posY = base->posY + self->cursorOffset[1] * base->scaleY;
}

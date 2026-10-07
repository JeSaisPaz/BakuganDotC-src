// bdc 0x089ee9cc UiSpriteMngMove
#include "bdc.h"

/* Adds (dx, dy, dz) to the sprite position (`posX/posY/posZ`). Returns 1/0. `ScriptOpSprite`
   cmd 7. */

int UiSpriteMngMove(UiSpriteMng *self, int slot, int dx, int dy, int dz)

{
  int ok;
  GfxSprite *spr;

  ok = 0;
  if ((self->layer != (void *)0x0) && (spr = self->sprites[slot], spr != (GfxSprite *)0x0)) {
    spr->posX = spr->posX + (float)dx;
    ok = 1;
    spr->posY = spr->posY + (float)dy;
    spr->posZ = spr->posZ + (float)dz;
  }
  return ok;
}

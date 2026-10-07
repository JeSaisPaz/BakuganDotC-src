// bdc 0x089ee7dc UiSpriteMngSetPos
#include "bdc.h"

/* Sets the position of a sprite (`posX/posY/posZ`, converted from int). Returns 1 on success,
   0 when the manager has no layer or the slot is empty. `ScriptOpSprite` cmd 4. */

int UiSpriteMngSetPos(UiSpriteMng *self, int slot, int x, int y, int z)
{
  GfxSprite *sprite;

  if (self->layer == (void *)0x0) {
    return 0;
  }
  sprite = self->sprites[slot];
  if (sprite == (GfxSprite *)0x0) {
    return 0;
  }
  sprite->posX = (float)x;
  sprite->posY = (float)y;
  sprite->posZ = (float)z;
  return 1;
}

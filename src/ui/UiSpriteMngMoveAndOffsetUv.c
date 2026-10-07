// bdc 0x089ee8e8 UiSpriteMngMoveAndOffsetUv
#include "bdc.h"

/* Adds (dx, dy, dz) to the position of sprite-manager slot `slot` (like `UiSpriteMngMove`) and
   scrolls its UV rectangle by (a, b, c, d) with `UiSpriteAddUvRect`. Returns 1 on success, 0
   otherwise. */

int UiSpriteMngMoveAndOffsetUv(UiSpriteMng *self, int slot, int dx, int dy, int dz, int a, int b, int c, int d)

{
  int ok;
  GfxSprite *sprite;
  float delta[4];
  
  ok = 0;
  if ((self->layer != (void *)0x0) && (sprite = self->sprites[slot], sprite != (GfxSprite *)0x0)) {
    delta[0] = (float)a;
    delta[1] = (float)b;
    delta[2] = (float)c;
    delta[3] = (float)d;
    sprite->posX = sprite->posX + (float)dx;
    sprite->posY = sprite->posY + (float)dy;
    sprite->posZ = sprite->posZ + (float)dz;
    UiSpriteAddUvRect(sprite,delta);
    ok = 1;
  }
  return ok;
}


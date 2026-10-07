// bdc 0x089ee658 UiSpriteMngSetRectPos
#include "bdc.h"

/* Sets a sprite's source rectangle (x=`rx`, y=`ry`, right=`rx+rw`, bottom=`ry+rh`, `UiSpriteSetUvRect`)
   and size (`UiSpriteSetSize(rw, rh)`), and its position (`posX` x, `posY` y, `posZ` z+200). For
   quad modes 1 and 3 (`quadMode`) the position is shifted by half the size (centre anchor).
   Returns 1 on success, 0 when the manager has no layer or the slot is empty. `ScriptOpSprite` cmd 3. */

int UiSpriteMngSetRectPos(UiSpriteMng *self, int slot, int x, int y, int z, int rx, int ry, int rw, int rh)

{
  GfxSprite *sprite;
  int mode;
  float rect[4];

  if (self->layer == NULL) {
    return 0;
  }
  sprite = self->sprites[slot];
  if (sprite == NULL) {
    return 0;
  }
  rect[0] = (float)rx;
  rect[1] = (float)ry;
  rect[2] = (float)(rx + rw);
  rect[3] = (float)(ry + rh);
  UiSpriteSetUvRect(sprite, rect);
  UiSpriteSetSize((float)rw, (float)rh, sprite);
  mode = sprite->quadMode;
  sprite->posX = (float)x;
  sprite->posY = (float)y;
  sprite->posZ = (float)(z + 200);
  if (mode < 2) {
    if (mode <= 0) {
      return 1;
    }
  }
  else if (mode != 3) {
    return 1;
  }
  sprite->posX = sprite->posX + (float)rw * 0.5f;
  sprite->posY = sprite->posY + (float)rh * 0.5f;
  return 1;
}

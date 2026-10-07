// bdc 0x08a2c19c GameGimmickCollidableGetPos
#include "bdc.h"

/* Vtable `0x08af2e2c` entry 18 (`+0x94`) of the collidable gimmick (`GameGimmickCollidableCtor`):
   copies the vec4 `+0x1d0` (inside its collision shape record) to `out`. */

void GameGimmickCollidableGetPos(GameGimmickCollidable *gimmick, float *out)

{
  out[0] = gimmick->hitPos.x;
  out[1] = gimmick->hitPos.y;
  out[2] = gimmick->hitPos.z;
  out[3] = gimmick->hitPos.w;
}

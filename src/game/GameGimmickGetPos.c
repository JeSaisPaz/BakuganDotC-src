// bdc 0x088d93a8 GameGimmickGetPos
#include "bdc.h"

/* Gimmick vtable slot 18 (shared by several gimmick classes): copies the gimmick position vec4
   `+0x20` to `out`. */

void GameGimmickGetPos(GameGimmick *gimmick, float *out)

{
  out[0] = gimmick->base.pos[0];
  out[1] = gimmick->base.pos[1];
  out[2] = gimmick->base.pos[2];
  out[3] = gimmick->base.pos[3];
}

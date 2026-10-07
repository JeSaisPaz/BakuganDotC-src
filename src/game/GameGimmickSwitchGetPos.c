// bdc 0x088db92c GameGimmickSwitchGetPos
#include "bdc.h"

/* Vtable `0x08af3734` slot 18 of the switch gimmick: copies the vec4 `+0x1d0` (the switch collider
   position) to `out`. */

void GameGimmickSwitchGetPos(GameGimmickSwitch *gimmick, float *out)

{
  out[0] = gimmick->pressPos.x;
  out[1] = gimmick->pressPos.y;
  out[2] = gimmick->pressPos.z;
  out[3] = gimmick->pressPos.w;
}

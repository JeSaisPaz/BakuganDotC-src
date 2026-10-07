// bdc 0x088dbb3c GameGimmickSwitchDisable
#include "bdc.h"

/* Vtable `0x08af3734` slot 16: when active (`+0x15e`), clears the flag, puts the switch in the off
   pose (`GameGimmickSwitchSetPose`) and, unless `silent`, plays sound `0x2c0002c`. */

void GameGimmickSwitchDisable(GameGimmickSwitch *gimmick, u8 silent)
{
  if (gimmick->base.active != 0) {
    gimmick->base.active = 0;
    GameGimmickSwitchSetPose(gimmick, 0);
    if (silent == 0 && SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x2c0002c, 0, 0);
    }
  }
}

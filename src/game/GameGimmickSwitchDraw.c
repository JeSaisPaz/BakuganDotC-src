// bdc 0x088dbb20 GameGimmickSwitchDraw
#include "bdc.h"

/* Draw method of the switch gimmick (vtable `0x08af3734` slot 8): forwards to
   `GfxModelDlWriteState`. */

void GameGimmickSwitchDraw(GameGimmickSwitch *gimmick, u32 **dl)

{
  GfxModelDlWriteState((GfxModel *)gimmick,dl);
  return;
}


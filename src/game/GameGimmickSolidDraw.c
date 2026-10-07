// bdc 0x088daf74 GameGimmickSolidDraw
#include "bdc.h"

/* Draw method of the solid gimmick (vtable `0x08af35d4` slot 8): forwards to
   `GfxModelDlWriteState`. */

void GameGimmickSolidDraw(GameGimmick *gimmick, u32 **dl)

{
  GfxModelDlWriteState(&gimmick->base,dl);
  return;
}


// bdc 0x088db300 GameGimmickTouchSpotDraw
#include "bdc.h"

/* Draw method of the touch-spot gimmick (vtable `0x08af3684` slot 8): forwards to
   `GfxModelDlWriteState`. */

void GameGimmickTouchSpotDraw(GameGimmickTouchSpot *gimmick, u32 **dl)

{
  GfxModelDlWriteState((GfxModel *)gimmick,dl);
  return;
}


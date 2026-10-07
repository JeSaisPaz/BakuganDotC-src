// bdc 0x088d9d8c GameGimmickBarrierDraw
#include "bdc.h"

/* Draw method of the barrier gimmick (vtable `0x08af33c4` slot 8): forwards to
   `GfxModelDlWriteState`. */

void GameGimmickBarrierDraw(GameGimmickBarrier *gimmick, u32 **dl)

{
  GfxModelDlWriteState((GfxModel *)gimmick,dl);
  return;
}


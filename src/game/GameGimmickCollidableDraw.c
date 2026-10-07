// bdc 0x088d51b8 GameGimmickCollidableDraw
#include "bdc.h"

/* Draw (vtable slot 8) of the breakable container gimmick (`GameGimmickCollidableCtor`, vtables
   `0x08af2e2c`/`0x08af2ecc`): forwards to `GfxModelDlWriteState`. */

void GameGimmickCollidableDraw(GameGimmickCollidable *obj, u32 **dl)

{
  GfxModelDlWriteState((GfxModel *)obj,dl);
  return;
}


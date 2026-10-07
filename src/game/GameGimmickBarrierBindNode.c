// bdc 0x088d9880 GameGimmickBarrierBindNode
#include "bdc.h"

/* Clears the vec4 at `+0x180` and binds the node callback `0x088d9854` with that vec4 to the
   barrier mesh node `gfx_102m__BA_CN` (`GfxModelSetMaterialAnimCallback`). Called by `GameGimmickBarrierCtor`. */

void GameGimmickBarrierBindNode(GameGimmickBarrier *gimmick)

{
  gimmick->texOffset[0] = 0.0;
  gimmick->texOffset[1] = 0.0;
  gimmick->texOffset[2] = 0.0;
  gimmick->texOffset[3] = 0.0;
  GfxModelSetMaterialAnimCallback
            ((GfxModel *)gimmick,"gfx_102m__BA_CN",GameGimmickBarrierDlWriteTexOffsetU,
             gimmick->texOffset);
  return;
}


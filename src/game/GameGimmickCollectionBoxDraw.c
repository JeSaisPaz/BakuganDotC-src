// bdc 0x088d64ec GameGimmickCollectionBoxDraw
#include "bdc.h"

/* Draw (vtable slot 8) of the collection box gimmick (`GameGimmickCollectionBoxCtor`, vtables
   `0x08af2f8c`/`0x08af302c`): `GfxModelDlWriteState`. */

void GameGimmickCollectionBoxDraw(GameGimmickCollectionBox *obj, u32 **dl)

{
  GfxModelDlWriteState((GfxModel *)obj,dl);
  return;
}


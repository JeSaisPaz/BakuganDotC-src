// bdc 0x088d8354 GameGimmickCameraDraw
#include "bdc.h"

/* Draw (vtable slot 8) of the surveillance camera gimmick (`GameGimmickCameraCtor`, vtables
   `0x08af325c`/`0x08af3304`): `GfxModelDlWriteState`. */

void GameGimmickCameraDraw(GameGimmickCamera *obj, u32 **dl)

{
  GfxModelDlWriteState((GfxModel *)obj,dl);
  return;
}


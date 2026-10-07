// bdc 0x088d7ea0 GameGimmickJetDoorDraw
#include "bdc.h"

/* Draw (vtable slot 8) of the Marucho-jet cabin door gimmick (`GameGimmickJetDoorCtor`, vtables
   `0x08af31ac`/`0x08af324c`): `GfxModelDlWriteState`. */

void GameGimmickJetDoorDraw(GameGimmickJetDoor *obj, u32 **dl)

{
  GfxModelDlWriteState((GfxModel *)obj,dl);
  return;
}


// bdc 0x088d7e64 GameGimmickJetDoorUpdate
#include "bdc.h"

/* Update (vtable slot 7) of the Marucho-jet cabin door gimmick (`GameGimmickJetDoorCtor`, vtables
   `0x08af31ac`/`0x08af324c`): `GameGimmickJetDoorUpdateOpen`, model animation (`GfxModelUpdateMotion`,
   `GfxModelApplyMotion`), `GameGimmickJetDoorUpdateLights`. */

void GameGimmickJetDoorUpdate(GameGimmickJetDoor *obj)

{
  GameGimmickJetDoorUpdateOpen(obj);
  GfxModelUpdateMotion((GfxModel *)obj);
  GfxModelApplyMotion((GfxModel *)obj);
  GameGimmickJetDoorUpdateLights(obj);
  return;
}


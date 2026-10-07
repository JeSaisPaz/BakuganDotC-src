// bdc 0x088d64b8 GameGimmickCollectionBoxUpdate
#include "bdc.h"

/* Update (vtable slot 7) of the collection box gimmick (`GameGimmickCollectionBoxCtor`, vtables
   `0x08af2f8c`/`0x08af302c`): `GameGimmickCollectionBoxUpdateLid`, then advances the model
   animation (`GfxModelUpdateMotion`, `GfxModelApplyMotion`). */

void GameGimmickCollectionBoxUpdate(GameGimmickCollectionBox *obj)

{
  GameGimmickCollectionBoxUpdateLid(obj);
  GfxModelUpdateMotion((GfxModel *)obj);
  GfxModelApplyMotion((GfxModel *)obj);
  return;
}


// bdc 0x088d82f4 GameGimmickCameraUpdate
#include "bdc.h"

/* Update (vtable slot 7) of the surveillance camera gimmick (`GameGimmickCameraCtor`, vtables
   `0x08af325c`/`0x08af3304`): `GameGimmickUpdate`, `GameGimmickCameraUpdateSprites`, then for
   states 0..2 the state handler (virtual slot 20) and the model animation (`GfxModelApplyMotion`). */

void GameGimmickCameraUpdate(GameGimmickCamera *obj)
{
  const VtblEntry *vt;
  int state;

  GameGimmickUpdate(&obj->base);
  GameGimmickCameraUpdateSprites(obj);
  state = obj->base.state;
  if (state >= 0 && state < 3) {
    vt = &((const VtblEntry *)obj->base.base.base.vtable)[20];
    ((void (*)(void *))vt->fn)((u8 *)obj + vt->delta);
    GfxModelApplyMotion((GfxModel *)obj);
  }
}

// bdc 0x088d5f00 GameGimmickSteamUpdate
#include "bdc.h"

/* Update (vtable slot 7) of the steam gimmick (`GameGimmickSteamCtor`, vtables
   `0x08af2edc`/`0x08af2f7c`): `GameGimmickUpdate`, then a frame counter `+0x18c`: after off+on
   frames it starts steam effect 0x32 at the model (`GfxEffectSpawnAttached`) and makes the
   collider solid (clears bit 2 of `collider+0x130`); after the on time it stops the effect
   (`GfxEffectStopAttached`) and makes it passable again. */

void GameGimmickSteamUpdate(GameGimmickSteam *obj)

{
  GameGimmickUpdate(&obj->base);
  obj->timer = obj->timer + 1;
  if (obj->venting == 0) {
    if (obj->offFrames + obj->onFrames <= obj->timer) {
      GfxEffectSpawnAttached(g_worldEffectMgr, 0x32, &obj->base.base.data->rootMatrix[0xc]);
      ((CollisionCollider *)obj->base.attached)->flags &= ~4u;
      obj->venting = 1;
      obj->timer = 0;
    }
  } else if (obj->onFrames <= obj->timer) {
    GfxEffectStopAttached(g_worldEffectMgr, 0x32, &obj->base.base.data->rootMatrix[0xc]);
    ((CollisionCollider *)obj->base.attached)->flags |= 4;
    obj->venting = 0;
  }
}

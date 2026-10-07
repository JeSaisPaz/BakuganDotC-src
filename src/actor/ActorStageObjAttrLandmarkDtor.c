// bdc 0x088a6a0c ActorStageObjAttrLandmarkDtor
#include "bdc.h"

/* Destructor (`g_actorStageObjAttrLandmarkVtbl` slot 1) of the hologram landmark
   (`ActorStageObjAttrLandmarkCtor`): reinstalls its vtable, stops the world effects attached to
   the model's root-matrix translation row and to the three `effectPos` slots
   (`GfxEffectStopAttached` on `g_worldEffectMgr`) and those it owns on `g_worldEffectMgr`
   and `g_btlUnitEffectMgr` (`GfxEffectStopOwned`), re-validates `unit` against the battle
   Bakugan list (`BtlBakuganListFind`, NULL when gone), frees `collisionBox` and runs
   `ActorStageObjBaseDtor`. GCC 2.x deleting destructor: frees the object when bit 0 of `flags`
   is set. Does nothing for a NULL `self`. */

void ActorStageObjAttrLandmarkDtor(ActorStageObjAttrLandmark *self, u32 flags)
{
  void *box;

  if (self == NULL) {
    return;
  }
  self->base.base.base.vtable = &g_actorStageObjAttrLandmarkVtbl;
  GfxEffectStopAttached(g_worldEffectMgr, -1, self->base.base.data->rootMatrix + 12);
  GfxEffectStopAttached(g_worldEffectMgr, -1, self->effectPos[0]);
  GfxEffectStopAttached(g_worldEffectMgr, -1, self->effectPos[1]);
  GfxEffectStopAttached(g_worldEffectMgr, -1, self->effectPos[2]);
  GfxEffectStopOwned(g_worldEffectMgr, -1, self);
  GfxEffectStopOwned(g_btlUnitEffectMgr, -1, self);
  self->unit = BtlBakuganListFind(self->unit);
  box = self->collisionBox;
  if (box != NULL) {
    MemLock();
    MemFree(box, NULL, 0);
    MemUnlock();
    self->collisionBox = NULL;
  }
  ActorStageObjBaseDtor(&self->base, 0);
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(self, NULL, 0);
    MemUnlock();
  }
}

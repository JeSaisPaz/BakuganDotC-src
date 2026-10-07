// bdc 0x088a5ccc ActorStageObjMineDtor
#include "bdc.h"

/* Destructor (vtable `0x08af2674` slot 1) of the mine stage object (`ActorStageObjMineCtor`):
   stops the effects it owns on the effect manager `0x08abd5b0` (`GfxEffectStopOwned(mgr, -1, obj)`)
   and runs `ActorStageObjBaseDtor`. (GCC 2.x deleting destructor: frees the object when bit 0 of
   `flags` is set). */

void ActorStageObjMineDtor(ActorStageObjMine *self, u32 flags)

{
  if (self != (ActorStageObjMine *)0x0) {
    (self->base).base.base.vtable = g_actorStageObjMineVtbl;
    GfxEffectStopOwned(g_btlUnitEffectMgr,-1,self);
    ActorStageObjBaseDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}


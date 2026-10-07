// bdc 0x088a53a0 ActorStageObjEggCrystalElementDtor
#include "bdc.h"

/* Destructor (vtable `0x08af25c4` slot 1) of the attribute egg crystal
   (`ActorStageObjEggCrystalElementCtor`): reinstalls its vtable and chains to
   `ActorStageObjEggCrystalDtor`. (GCC 2.x deleting destructor: frees the object when bit 0 of
   `flags` is set). */

void ActorStageObjEggCrystalElementDtor(ActorStageObjEggCrystal *self, u32 flags)

{
  if (self != (ActorStageObjEggCrystal *)0x0) {
    (self->base).base.base.vtable = g_actorStageObjEggCrystalElementVtbl;
    ActorStageObjEggCrystalDtor(self,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}


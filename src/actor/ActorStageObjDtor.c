// bdc 0x088a8c34 ActorStageObjDtor
#include "bdc.h"

/* Destructor (vtable `g_actorStageObjVtbl` slot 1) of the HP stage object (`ActorStageObjCtor`): refreshes
   `+800` through `BtlBakuganListFind` and runs `ActorStageObjBaseDtor`. (GCC 2.x deleting
   destructor: frees the object when bit 0 of `flags` is set). */

void ActorStageObjDtor(ActorStageObj *self, u32 flags)

{
  void *found;

  if (self != (ActorStageObj *)0x0) {
    (self->base).base.base.vtable = g_actorStageObjVtbl;
    found = BtlBakuganListFind(self->unit);
    self->unit = found;
    ActorStageObjBaseDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}


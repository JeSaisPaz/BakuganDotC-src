// bdc 0x088b0724 ActorStageObjScreenDtor
#include "bdc.h"

/* Destructor (vtable `0x08af2a44` slot 1) of the screen stage object (`ActorStageObjScreenCtor`):
   chains to `ActorStageObjBaseDtor`. (GCC 2.x deleting destructor: frees the object when bit 0 of
   `flags` is set). */

void ActorStageObjScreenDtor(ActorStageObjScreen *self, u32 flags)

{
  if (self != (ActorStageObjScreen *)0x0) {
    (self->base).base.base.vtable = g_actorStageObjScreenVtbl;
    ActorStageObjBaseDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}


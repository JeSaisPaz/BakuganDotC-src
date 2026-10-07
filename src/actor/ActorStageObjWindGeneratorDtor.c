// bdc 0x088a65cc ActorStageObjWindGeneratorDtor
#include "bdc.h"

/* Destructor (vtable `0x08af2714` slot 1) of the wind generator
   (`ActorStageObjWindGeneratorCtor`): chains to `ActorStageObjBaseDtor`. (GCC 2.x deleting
   destructor: frees the object when bit 0 of `flags` is set). */

void ActorStageObjWindGeneratorDtor(ActorStageObjWindGenerator *self, u32 flags)

{
  if (self != (ActorStageObjWindGenerator *)0x0) {
    (self->base).base.base.vtable = g_actorStageObjWindGeneratorVtbl;
    ActorStageObjBaseDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}


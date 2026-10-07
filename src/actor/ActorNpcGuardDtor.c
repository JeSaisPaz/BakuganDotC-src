// bdc 0x08a2c588 ActorNpcGuardDtor
#include "bdc.h"

/* Destructor (vtable `0x08af4024` entry 1) of the guard class (`ActorNpcGuardCtor`, models
   0x4e/0x50): reinstalls the guard vtable, runs `ActorNpcDtor` and frees the object when `flags &
   1`. */

void ActorNpcGuardDtor(ActorNpcGuard *self, u32 flags)

{
  if (self != (ActorNpcGuard *)0x0) {
    (self->base).base.base.base.vtable = (void *)g_actorNpcGuardVtbl;
    ActorNpcDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}


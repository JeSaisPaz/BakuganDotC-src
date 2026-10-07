// bdc 0x088e54f4 ActorNpcCloakDtor
#include "bdc.h"

/* Destructor of the cloaked guard class (model 0x4f, `ActorNpcCloakCtor`, vtable `0x08af39e4`)
   slot 1: installs vtable `0x08af4024` and runs the NPC destructor `ActorNpcDtor`. */

void ActorNpcCloakDtor(ActorNpcCloak *self, u32 flags)

{
  if (self != (ActorNpcCloak *)0x0) {
    (self->base).base.base.base.base.vtable = (void *)g_actorNpcGuardVtbl;
    ActorNpcDtor((ActorNpc *)self,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}


// bdc 0x088e5c50 ActorNpcDtor
#include "bdc.h"

/* Destructor of the field NPC/guard classes (base `ActorNpcCtor`), vtable `g_actorNpcVtbl` slot 1:
   destroys the view-cone object `+0x418` (`ActorNpcViewConeDtor`), clears the head effects
   (`ActorNpcShowHeadEffect`) and runs `ActorDtor`. */

void ActorNpcDtor(ActorNpc *self, u32 flags)

{
  void *cone;
  
  if (self != (ActorNpc *)0x0) {
    cone = self->viewCone;
    (self->base).base.base.vtable = (void *)&g_actorNpcVtbl;
    if (cone != (void *)0x0) {
      ActorNpcViewConeDtor(cone,3);
      self->viewCone = (void *)0x0;
    }
    ActorNpcShowHeadEffect(self,-1,'\x01','\x01');
    ActorDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}


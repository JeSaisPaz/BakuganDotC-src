// bdc 0x08a2c61c ActorNpcRobotDtor
#include "bdc.h"

/* Destructor (vtable `0x08af3e94` entry 1) of the robot class (`ActorNpcRobotCtor`, models
   0x51..0x53): reinstalls the robot vtable, runs `ActorNpcDtor` and frees the object when `flags
   & 1`. */

void ActorNpcRobotDtor(ActorNpc *self, u32 flags)

{
  if (self != (ActorNpc *)0x0) {
    (self->base).base.base.vtable = (void *)g_actorNpcRobotVtbl;
    ActorNpcDtor(self,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}


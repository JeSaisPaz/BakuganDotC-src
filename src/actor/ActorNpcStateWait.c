// bdc 0x088e742c ActorNpcStateWait
#include "bdc.h"

/* AI state 1 of the field NPC/guard classes (base `ActorNpcCtor`) (slot 36): only the interrupt
   check (`ActorNpcCheckInterrupt`); the wait timer is handled by the patrol code. */

void ActorNpcStateWait(ActorNpc *self)

{
  ActorNpcCheckInterrupt(self,'\0');
  return;
}


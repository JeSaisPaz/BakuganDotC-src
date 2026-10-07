// bdc 0x088e7ac4 ActorNpcStateInvestigateBase
#include "bdc.h"

/* AI state 5 of the base NPC class (slot 40): unless interrupted, returns to state 0 and clears
   `+0x400` (the guard classes override it). */

void ActorNpcStateInvestigateBase(ActorNpc *self)

{
  s32 interrupted;
  
  interrupted = ActorNpcCheckInterrupt(self,'\0');
  if (interrupted == 0) {
    self->aiState = 0;
    self->subStep = 0;
    self->forceUpdate = '\0';
  }
  return;
}


// bdc 0x088df198 ActorResetBehaviour
#include "bdc.h"

/* Vtable slot 13 of the base actor: clears `+0x354`/`+0x355`, selects behaviour 0 (`ActorSetStateBase`),
   looks up the route (`ActorLookupRoute`), resets the route step `+0x360` and switches to
   behaviour 9 (follow route) when a route exists. */

void ActorResetBehaviour(Actor *self)

{
  self->detected = '\0';
  self->alerted = '\0';
  ActorSetStateBase(self,0,'\0');
  ActorLookupRoute(self);
  self->routeStep = 0;
  if (self->route != (s32 *)0x0) {
    ActorSetStateBase(self,9,'\0');
  }
  return;
}


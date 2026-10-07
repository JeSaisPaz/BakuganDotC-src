// bdc 0x08a2c410 ActorEnterSavePlacementState
#include "bdc.h"

/* Actor virtual (vtable entry 15, offset `+0x7c`, in the base actor vtable `0x08af37e4` and the
   player vtable `0x08af38e4`): switches to state 8 with `ActorSetStateBase``(actor, 8, 0)`, which
   saves the placement (`ActorSavePlacement`) and stops the actor. */

void ActorEnterSavePlacementState(Actor *self)

{
  ActorSetStateBase(self,8,'\0');
  return;
}


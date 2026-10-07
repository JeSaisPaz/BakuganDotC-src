// bdc 0x088e7b08 ActorNpcState07Reset
#include "bdc.h"

/* AI state 7 handler of the NPC classes (slot 42): returns to state 0 and clears the sub-step
   `+0x3a8`. */

void ActorNpcState07Reset(ActorNpc *self)

{
  self->aiState = 0;
  self->subStep = 0;
  return;
}


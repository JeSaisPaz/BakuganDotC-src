// bdc 0x088df1f8 ActorSlot15
#include "bdc.h"

/* Vtable slot 15 of the actor classes (7 vtables): queues the actor for deferred deletion via `CoreObjectDeferDelete(actor, 0)`.
    */

void ActorSlot15(Actor *self)

{
  CoreObjectDeferDelete((CoreObject *)self,0);
  return;
}


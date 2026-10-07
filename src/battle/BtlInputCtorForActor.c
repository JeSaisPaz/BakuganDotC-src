// bdc 0x0888483c BtlInputCtorForActor
#include "bdc.h"

/* Constructs the input controller of a field actor: like `BtlInputCtorForUnit` but with
   `mode = 1`, so `BtlInputReadActions` reads the actor's fields and adds the actor-only action
   bits. Installs `g_btlInputVtbl` and resets the state (`BtlInputReset`). Called by
   `ActorCtor`. */
void BtlInputCtorForActor(BtlInput *self, void *actor)
{
    self->vtbl = g_btlInputVtbl;
    self->owner = actor;
    self->mode = 1;
    BtlInputReset(self);
}

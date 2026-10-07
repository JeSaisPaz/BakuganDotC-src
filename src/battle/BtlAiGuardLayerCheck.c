// bdc 0x088934ac BtlAiGuardLayerCheck
#include "bdc.h"

/* Check method of behaviour layer 0 of `BtlAi` (guard/stagger reaction,
   `MemberFnPtr` at `0x08a802e0`): sets the guard layer's `active` byte when the owner has any
   of the state flags 0x30000000 or is in state 3..6 (the code also tests 3, 4 and 5 separately),
   clears it otherwise, then calls the layer's virtual entry 2. */
void BtlAiGuardLayerCheck(BtlAi *self)
{
    BtlBakugan *owner = self->owner;
    const VtblEntry *entry;
    bool flagged;
    bool inRange;

    flagged = (owner->stateFlags & 0x30000000) != 0 || owner->state == 4 || owner->state == 5;
    inRange = owner->state > 2 && owner->state < 7;
    self->guard.base.active = flagged || inRange || owner->state == 3;

    entry = &self->guard.base.vtbl[2];
    ((void (*)(void *))entry->fn)((u8 *)&self->guard + entry->delta);
}

// bdc 0x089d90c0 CoreNodeOwnerDtor
#include "bdc.h"

/* Destructor of `CoreNodeOwner` (`g_coreNodeOwnerVtbl` entry 1): resets the vtable, destroys
   every owned node with `CoreNodeDestroyChain` when the list is non-empty, runs `CoreNodeDtor`
   and frees the object when bit 0 of `flags` is set. A NULL `owner` is ignored. */
void CoreNodeOwnerDtor(CoreNodeOwner *owner, u32 flags)
{
    if (owner == NULL) {
        return;
    }
    owner->vtable = g_coreNodeOwnerVtbl;
    if (owner->head != NULL) {
        CoreNodeDestroyChain(owner->head);
    }
    CoreNodeDtor((CoreNode *)owner, 0);
    if (flags & 1) {
        MemLock();
        MemFree(owner, NULL, 0);
        MemUnlock();
    }
}

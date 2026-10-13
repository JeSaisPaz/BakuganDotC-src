// bdc 0x089d7318 Mem2ContainsPtr
#include "bdc.h"

/* Range-check virtual of `MemMng2` (vtable slot `+0x1c`): returns true when `ptr` lies in
   `[pool, pool + totalSize]` (both ends inclusive) of the state block, false when there is no
   state block or `ptr` is outside. */
s32 Mem2ContainsPtr(MemMng2 *self, void *ptr)
{
    MemMng *state = self->state;
    u8 *base;

    if (state == NULL) {
        return 0;
    }
    base = (u8 *)state->pool;
    if ((u8 *)ptr < base) {
        return 0;
    }
    if (base + state->totalSize < (u8 *)ptr) {
        return 0;
    }
    return 1;
}

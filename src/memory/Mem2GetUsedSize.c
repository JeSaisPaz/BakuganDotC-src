// bdc 0x089d7398 Mem2GetUsedSize
#include "bdc.h"

/* Returns the number of bytes currently allocated from the Mem2 allocator (its state block's
   `usedSize`), or 0 if it has no state. */
u32 Mem2GetUsedSize(MemMng2 *self)
{
    if (self->state == NULL) {
        return 0;
    }
    return self->state->usedSize;
}

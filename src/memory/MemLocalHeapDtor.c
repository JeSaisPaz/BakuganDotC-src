// bdc 0x08a016c4 MemLocalHeapDtor
#include "bdc.h"

/* Destructor of a `MemLocalHeap`: reinstalls `g_memLocalHeapVtbl`, zeroes the `MemMng` heap
   header (0x40 bytes on the PSP) at the start of the buffer and frees the heap object (under `MemLock`) when
   `flags & 1`. Does nothing for NULL. */
void MemLocalHeapDtor(MemLocalHeap *self, u32 flags)
{
    if (self == NULL) {
        return;
    }
    self->vtbl = g_memLocalHeapVtbl;
    memset(self->state, 0, sizeof(MemMng));
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}

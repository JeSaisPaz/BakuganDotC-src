// bdc 0x089d6e4c Mem2Destroy
#include "bdc.h"

/* Destructor of `MemMng2`: reinstalls `g_memMng2Vtbl`, frees the record array and the state
   block (each under `MemLock`) and clears the pointers, and frees the object itself when bit 0
   of `flags` is set. Does nothing for NULL. */
void Mem2Destroy(MemMng2 *self, u32 flags)
{
    if (self == NULL) {
        return;
    }
    self->vtbl = g_memMng2Vtbl;
    if (self->records != NULL) {
        MemLock();
        MemFree(self->records, NULL, 0);
        MemUnlock();
        self->records = NULL;
    }
    if (self->state != NULL) {
        MemLock();
        MemFree(self->state, NULL, 0);
        MemUnlock();
        self->state = NULL;
    }
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}

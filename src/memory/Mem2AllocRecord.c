// bdc 0x089d7408 Mem2AllocRecord
#include "bdc.h"

/* Takes an unused `MemBlock` record (one whose `data` is NULL) from the allocator's record array:
   tries at most `recordCount` slots starting at `recordHint`, advancing the hint past every slot
   tried (wrapping to 0 only once it exceeds `recordCount`, so slot `recordCount` itself can be
   probed). Returns the record, or NULL if none of the tried slots is free. */
MemBlock *Mem2AllocRecord(MemMng2 *self)
{
    s32 count = self->recordCount;
    s32 tries;

    for (tries = 0; tries < count; tries++) {
        s32 index = self->recordHint;

        self->recordHint = index + 1;
        if (self->records[index].data == NULL) {
            return &self->records[index];
        }
        if (index + 1 > count) {
            self->recordHint = 0;
        }
    }
    return NULL;
}

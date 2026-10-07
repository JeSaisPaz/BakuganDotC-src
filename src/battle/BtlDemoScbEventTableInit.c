// bdc 0x08907e20 BtlDemoScbEventTableInit
#include "bdc.h"

/* Initialises the event table of a `.scb` scene: for each of the nine per-event-kind slots
   `lists[0..8]` (+0x18..+0x38), in order, allocates a 12-byte `CoreObjectList` head from the low
   heap (saving and restoring the heap's placement mode under `MemLock`) and zeroes its
   `tail`, `head` and `count`. The allocation result is not checked for NULL. Called by
   `BtlDemoScbEventTableCtor`. */
void BtlDemoScbEventTableInit(BtlDemoScbEventTable *table)
{
    s32 i;

    for (i = 0; i < 9; i++) {
        bool wasLow;
        CoreObjectList *list;

        MemLock();
        wasLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        list = (CoreObjectList *)MemAlloc(sizeof(CoreObjectList), NULL, 0);
        MemSetAllocFromLow(wasLow);
        MemUnlock();
        table->lists[i] = list;
        table->lists[i]->tail = NULL;
        table->lists[i]->head = NULL;
        table->lists[i]->count = 0;
    }
}
